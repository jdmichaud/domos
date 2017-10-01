/**
 * Mosaic of camera
 */
import { Component, OnInit } from '@angular/core';
import { Camera } from 'classes/camera';
import { CameraService } from 'services/cameras.service';

@Component({
  selector: 'cameras-list',
  templateUrl: './cameras-list.component.html',
  providers: [CameraService],
})
export class CamerasList implements OnInit {
  private cameras: Camera[];

  constructor(private cameraService: CameraService) {}

  public retrieveCameras(): void {
    this.cameraService.getCameras().then((cameras: Camera[]): Camera[] => this.cameras = cameras);
  }

  public ngOnInit(): void {
    this.retrieveCameras();
  }
}
