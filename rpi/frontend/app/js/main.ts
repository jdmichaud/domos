/**
 * Main file
 */

import 'jquery';

import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap';

import 'resources/css/navbar-fixed-top.css';

import { NgModule } from '@angular/core';

import { AppComponent } from 'js/app.component';

@NgModule({
  declarations: [AppComponent],
  bootstrap: [AppComponent],
})
class MainModule { }
