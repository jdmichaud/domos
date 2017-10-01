/**
 * Service to perform REST requests on camera resources]
 */

import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import { Camera } from 'classes/camera';

@Injectable()
export class CameraService {
  constructor(private http: Http) {}

  public getCameras(): Promise<Camera[]> {
    return Promise.resolve([new Camera('192.168.0.10', 'F8:E7:89:AC')]);
  }
}
