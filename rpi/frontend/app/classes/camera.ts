/**
 * The class representaing a Camera
 */
export class Camera {
  public ip: string;
  public mac: string;

  constructor(ip: string, mac: string) {
    this.ip = ip;
    this.mac = mac;
  }
}
