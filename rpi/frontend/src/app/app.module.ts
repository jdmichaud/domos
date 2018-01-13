/**
 * App Module
 */

// import 'jquery';
// import 'bootstrap/dist/js/bootstrap';

import { NgModule } from '@angular/core';
import { HttpModule } from '@angular/http';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { CameraComponent } from './components/camera/camera.component';
import { CamerasList } from './components/cameras-list/cameras-list.component';
import { CameraService } from './services/cameras.service';

@NgModule({
  declarations: [
    AppComponent,
    CamerasList,
    CameraComponent,
  ],
  imports: [
    BrowserModule,
    HttpModule,
  ],
  bootstrap: [AppComponent],
})
export class AppModule { }
