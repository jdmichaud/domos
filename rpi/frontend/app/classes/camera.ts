/**
 * The class representaing a Camera
 */
export class Camera {
  public id: number;
  public ip: string;
  public mac: string;

  constructor(id: number, ip: string, mac: string) {
    this.id = id;
    this.ip = ip;
    this.mac = mac;
  }
}
