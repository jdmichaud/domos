/**
 * Service to perform REST requests on camera resources]
 */

import { Injectable } from '@angular/core';
import { Http, Response } from '@angular/http';
import 'rxjs/add/operator/toPromise';
import { Observable } from 'rxjs/Observable';
import { Observer } from 'rxjs/Observer';

import { Camera } from 'app/classes/camera';

@Injectable()
export class CameraService {
  private observable: Observable<Camera[]>;
  private CAMERA_URL: string = `http://${location.hostname}:12000/api/camera/`;

  constructor(private http: Http) {}

  public getCameras(): Observable<Camera[]> {
    this.observable = this.observable || Observable.create(
      (observer: Observer<Camera[]>): void => {
        this.getPromise(observer, true);
      });

    return this.observable;
  }

  public del(id: number): Promise<Response> {
    return this.http.delete(`${this.CAMERA_URL}/${id}`).toPromise();
  }

  private getPromise(observer: Observer<Camera[]>, initialCall: boolean): any {
    const promise: Promise<Response> = this.http
      // Call with ?watch starting with the recursive call so
      // we can initialize the list
      .get(this.CAMERA_URL + (initialCall ? '' : '?watch'))
      .toPromise();

    return promise.then(response => {
      observer.next(response.json() as Camera[]);

      return this.getPromise(observer, false);
    }).catch(error => {
      if (error.status !== 0) {
        console.error('error while waiting for camera service:', error);
      } // Is status === 0, its probably a long-polling timeout

      return this.getPromise(observer, false);
    });
  }
}
