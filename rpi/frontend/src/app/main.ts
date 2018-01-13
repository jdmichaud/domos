/**
 * Main file
 */
// Those because of some weird error. Angular2 is really not ready for prod...
import 'reflect-metadata';
import 'zone.js';

import { enableProdMode } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { AppModule } from 'app/app.module';

// enableProdMode();

platformBrowserDynamic().bootstrapModule(AppModule);
