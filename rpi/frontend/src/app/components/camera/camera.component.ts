/**
 * Application main component
 */
import { AfterViewInit, Component, Input } from '@angular/core';

import { Camera } from 'app/classes/camera';
import { HlsService } from 'app/services/hls.service';

@Component({
  selector: 'camera',
  templateUrl: 'camera.component.html',
  providers: [HlsService],
})
export class CameraComponent implements AfterViewInit {
  @Input() public camera: Camera;

  constructor(private hlsService: HlsService) {}

  public ngAfterViewInit(): void {
    this.hlsService.registerCamera(this.camera.id, this.camera.ip);
  }
}
