/**
 * Service to wrap HlsJs library functionalities
 */

import { Inject, Injectable } from '@angular/core';
import * as Hls from 'hls.js';
import { CameraService } from 'services/cameras.service';

@Injectable()
export class HlsService {
  private cameraService: CameraService;
  constructor(@Inject(CameraService) cameraService: CameraService) {
    this.cameraService = cameraService;
  }

  public registerCamera(id: number, ip: string): void {
    if (Hls.isSupported()) {
      const videoelt: HTMLVideoElement =
        <HTMLVideoElement> document.getElementById(`camera-${id}`);
      const hls: Hls = new Hls();
      hls.config.liveMaxLatencyDuration = 1;
      hls.loadSource(`http://${ip}:9000/playlist.m3u8`);
      hls.attachMedia(videoelt);
      hls.on(Hls.Events.MANIFEST_PARSED, () => {
        videoelt.play();
      });
      hls.on(Hls.Events.ERROR, (event: string, eventData: any): void => {
        if (eventData.type === 'networkError' && eventData.details === 'levelLoadError') {
          this.cameraService.del(id).then(() => hls.destroy());
        }
      });
      // Object.keys(Hls.Events).forEach((e: string): void => {
      //   hls.on(Hls.Events[e], console.info.bind(console));
      // });
    } else {
      console.log('ERROR: Hls not supported in this browser');
    }
  }
}
