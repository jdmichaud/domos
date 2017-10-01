/**
 * Application main component
 */
import { Component, Input, OnInit } from '@angular/core';
import { Camera } from 'classes/camera';

@Component({
  selector: 'camera',
  templateUrl: './camera.component.html',
})
export class CameraComponent implements OnInit {
  @Input() camera: Camera;

  constructor() {}

  public ngOnInit(): void {}
}
