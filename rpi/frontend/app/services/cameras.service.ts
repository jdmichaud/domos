/**
 * Service to perform REST requests on camera resources]
 */

import { Injectable } from '@angular/core';
import { Http, Response } from '@angular/http';
import { Observable } from 'rxjs/Observable';
import { Observer } from 'rxjs/Observer';
import 'rxjs/add/operator/toPromise';

import { Camera } from 'classes/camera';

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
    const promise = this.http
      // Call with ?watch starting with the recursive call so
      // we can initialize the list
      .get(this.CAMERA_URL + (initialCall ? '' : '?watch'))
      .toPromise();

    return promise.then(response => {
      observer.next(response.json() as Camera[]);

      return this.getPromise(observer, false);
    }).catch(error => observer.error(error));
  }
}
