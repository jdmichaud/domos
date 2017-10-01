/**
 * Service to wrap HlsJs library functionalities
 */

import { Injectable } from '@angular/core';
import * as Hls from 'hls.js';

@Injectable()
export class HlsService {
  public registerCamera(id: number, ip: string): void {
    if (Hls.isSupported()) {
      const videoelt: HTMLVideoElement = 
        <HTMLVideoElement> document.getElementById(`camera-${id}`);
      const hls: Hls = new Hls();
      hls.config.liveMaxLatencyDuration = 1;
      hls.loadSource(`https://${ip}/playlist.m3u8`);
      hls.attachMedia(videoelt);
      hls.on(Hls.Events.MANIFEST_PARSED, () => videoelt.play());
    } else {
      console.log('ERROR: Hls not supported in this browser');
    }
  }
}
