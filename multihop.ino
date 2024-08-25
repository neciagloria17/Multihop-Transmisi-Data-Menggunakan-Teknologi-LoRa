TEST REP

#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 10         // Pin CS (Chip Select) untuk Dragino Shield
#define RST_PIN 9         // Pin RST untuk Dragino Shield
#define BAND 915E6        // Frekuensi LoRa (disesuaikan dengan wilayah Anda)
#define SF 10             // Spreading Factor
#define BW 125E3          // Bandwidth 125 kHz
#define CR 6              // Code Rate 4/6
#define CLIENT_ADDRESS 1  // Alamat pengirim
#define SERVER_ADDRESS 3  // Alamat tujuan

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("Gagal mulai LoRa!");
    while (1);
  }

  LoRa.setSpreadingFactor(SF);  // Atur Spreading Factor
  LoRa.setCodingRate4(CR);      // Atur Code Rate 4/6
  LoRa.setSignalBandwidth(BW);  // Atur Bandwidth 125 kHz

  Serial.println("LoRa Mulai!");
}



// void loop() {
//   // Tunggu hingga menerima paket
//   if (LoRa.parsePacket()) {
//     int senderAddress = LoRa.read();        // Baca alamat pengirim
//     int destinationAddress = LoRa.read();   // Baca alamat tujuan

//     if (destinationAddress != SERVER_ADDRESS) {
//       Serial.println("Alamat tujuan tidak sesuai!");
//       return;
//     }

//     String receivedData = "";
//     while (LoRa.available()) {
//       receivedData += (char)LoRa.read();
//     }

//     Serial.print("Data diterima dari node ");
//     Serial.print(senderAddress);
//     Serial.println(", meneruskan dengan penanda R1");

//     String modifiedData = receivedData + " R1";

//     LoRa.beginPacket();
//     LoRa.write(senderAddress);   // Tetap kirim alamat pengirim
//     LoRa.write(SERVER_ADDRESS);  // Kirim ke alamat tujuan yang benar
//     LoRa.print(modifiedData);
//     LoRa.endPacket();
//   }
// }

void loop() {
  // Tunggu hingga menerima paket
  if (LoRa.parsePacket()) {
    int senderAddress = LoRa.read();        // Baca alamat pengirim
    int destinationAddress = LoRa.read();   // Baca alamat tujuan

    if (destinationAddress != SERVER_ADDRESS) {
      Serial.println("Alamat tujuan tidak sesuai!");
      return;
    }

    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    // Debugging: Tampilkan data yang diterima di repeater
    Serial.print("Repeater menerima data dari node ");
    Serial.print(senderAddress);
    Serial.print(": ");
    Serial.println(receivedData);

    // Tambahkan penanda "R1" pada data
    String modifiedData = receivedData + " R1";

    // Debugging: Tampilkan data yang dikirim dari repeater
    Serial.print("Repeater mengirim ulang data ke receiver: ");
    Serial.println(modifiedData);

    // Kirim data yang dimodifikasi ke receiver
    LoRa.beginPacket();
    LoRa.write(senderAddress);   // Tetap kirim alamat pengirim
    LoRa.write(SERVER_ADDRESS);  // Kirim ke alamat tujuan yang benar
    LoRa.print(modifiedData);
    LoRa.endPacket();
  }
}