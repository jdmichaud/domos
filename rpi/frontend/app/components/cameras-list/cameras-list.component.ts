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
    this.cameraService.getCameras().subscribe(
      (cameras: Camera[]): void => {
        this.cameras = cameras;
      },
      (error) => {
        this.cameras = [];
      },
      () => console.log('ERROR: Connection completed unexpectedly'));
  }

  public ngOnInit(): void {
    this.retrieveCameras();
  }
}
