/**
 * Service to wrap HlsJs library functionalities
 */

import { Inject, Injectable } from '@angular/core';
import * as Hls from 'hls.js';
import { CameraService } from 'app/services/cameras.service';

@Injectable()
export class HlsService {
  private cameraService: CameraService;
  constructor(@Inject(CameraService) cameraService: CameraService) {
    this.cameraService = cameraService;
  }

  public registerCamera(id: number, ip: string): void {
    if (Hls.isSupported()) {
      let errcount: number = 0;
      let manifestParsed: boolean = false;
      let mediaAttached: boolean = false;
      const videoelt: HTMLVideoElement =
        <HTMLVideoElement> document.getElementById(`camera-${id}`);
      const hls: Hls = new Hls();
      hls.config.liveMaxLatencyDuration = 1;
      hls.loadSource(`http://${ip}:8000/playlist.m3u8`);
      hls.attachMedia(videoelt);
      hls.on(Hls.Events.MANIFEST_PARSED, () => {
        console.log('start playing video');
        manifestParsed = true;
        errcount = 0;
        videoelt.play();
      });
      hls.on(Hls.Events.MEDIA_ATTACHED, () => mediaAttached = true);
      hls.on(Hls.Events.ERROR, (event: string, eventData: any): void => {
        console.error('HLS error:', event, eventData);
        errcount += 1;
        if (errcount > 4) {
          this.cameraService.del(id).then(() => hls.destroy());
          console.error(`camera ${id} (${ip}) is unreachable: removed`);
        } else if (eventData.fatal) {
          switch (eventData.type) {
          case Hls.ErrorTypes.NETWORK_ERROR:
            console.log('fatal network error encountered, try to recover');
            if (manifestParsed && mediaAttached) {
              // Network error in the middle of the stream, retry.
              hls.startLoad();
            } else {
              // Network error at the beginning of the stream. The camera
              // is probably not fully started yet.
              console.log('Wait for camera to wake up...');
              setTimeout(() => hls.loadSource(`http://${ip}:8000/playlist.m3u8`), 10000);
            }
            break;
          case Hls.ErrorTypes.MEDIA_ERROR:
            console.log('fatal network error encountered, try to recover');
            hls.recoverMediaError();
            break;
          }
        }
      });
      Object.keys(Hls.Events).forEach((e: string): void => {
        hls.on(Hls.Events[e], console.log.bind(console));
      });
    } else {
      console.log('ERROR: Hls not supported in this browser');
    }
  }
}
