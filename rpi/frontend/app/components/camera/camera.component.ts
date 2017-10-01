/**
 * Application main component
 */
import { Component, Input, OnInit } from '@angular/core';

import { Camera } from 'classes/camera';
import { HlsService } from 'services/hls.service';

@Component({
  selector: 'camera',
  templateUrl: './camera.component.html',
  providers: [HlsService],
})
export class CameraComponent implements OnInit {
  @Input() private camera: Camera;
  @Input() private cameraIndex: number;

  constructor(private hlsService: HlsService) {}

  public ngOnInit(): void {
    this.hlsService.registerCamera(this.cameraIndex, this.camera.ip);
  }
}
