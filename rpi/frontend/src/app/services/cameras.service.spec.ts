/**
 *
 */
import { inject, TestBed } from '@angular/core/testing';
import { BaseRequestOptions, Http, HttpModule, XHRBackend } from '@angular/http';
import { MockBackend } from '@angular/http/testing';

import { CameraService } from './cameras.service';

describe('CameraService', () => {

  beforeEach(() => {

    TestBed.configureTestingModule({
      imports: [HttpModule],
      providers: [
        CameraService,
        { provide: XHRBackend, useClass: MockBackend },
      ],
    });

  });

  it('should return an Observable<Camera>', () => {
    inject([CameraService, XHRBackend], (cameraService: any, mockBackend: any) => {
      mockBackend.connections.subscribe((connection: any) => {
      // This is called every time someone subscribes to
      // an http call.
      //
      // Here we want to fake the http response.
      });
    });
  });
});
