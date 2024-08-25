#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 10         // Pin CS (Chip Select) untuk Dragino Shield
#define RST_PIN 9         // Pin RST untuk Dragino Shield
#define BAND 915E6        // Frekuensi LoRa (disesuaikan dengan wilayah Anda)
#define SF 10             // Spreading Factor
#define BW 125E3          // Bandwidth 125 kHz
#define CR 6              // Code Rate 4/6
#define SERVER_ADDRESS 3  // Alamat tujuan

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(BAND)) {
    Serial.println("Gagal mulai LoRa!");
    while (1);
  }

  LoRa.setSpreadingFactor(SF);      // Atur Spreading Factor
  LoRa.setCodingRate4(CR);          // Atur Code Rate 4/6
  LoRa.setSignalBandwidth(BW);      // Atur Bandwidth 125 kHz

  Serial.println("LoRa Mulai!");
}

// 

// void loop() {
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

//     bool isFromRepeater = receivedData.endsWith("R1");

//     Serial.print("Data diterima dari node ");
//     Serial.print(senderAddress);
//     Serial.print(": ");
//     Serial.println(receivedData);  // Debug output untuk semua data

//     // Pisahkan data menjadi suhu, kelembaban, dan SF jika format benar
//     int commaIndex1 = receivedData.indexOf(',');
//     int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
//     if (commaIndex1 != -1 && commaIndex2 != -1) {
//       float temperature = receivedData.substring(0, commaIndex1).toFloat();
//       float humidity = receivedData.substring(commaIndex1 + 1, commaIndex2).toFloat();
//       int sf = receivedData.substring(commaIndex2 + 1).toInt();

//       float snr = LoRa.packetSnr();
//       int rssi = LoRa.packetRssi();

//       Serial.print("Suhu = ");
//       Serial.print(temperature);
//       Serial.print(" °C, Kelembaban = ");
//       Serial.print(humidity);
//       Serial.print("%, SNR = ");
//       Serial.print(snr);
//       Serial.print(" dB, RSSI = ");
//       Serial.print(rssi);
//       Serial.print(" dBm");

//       if (isFromRepeater) {
//         Serial.print(", dari Repeater (R1)");
//       }

//       Serial.println();
//     } else {
//       Serial.println("Format data tidak valid!");
//     }
//   }
// }

void loop() {
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

    bool isFromRepeater = receivedData.endsWith("R1");

    // Jika data dari repeater, hapus penanda "R1" untuk parsing data
    if (isFromRepeater) {
      receivedData = receivedData.substring(0, receivedData.length() - 3);
    }

    // Pisahkan data menjadi suhu, kelembaban, SF, BW, CR
    int commaIndex1 = receivedData.indexOf(',');
    int commaIndex2 = receivedData.indexOf(',', commaIndex1 + 1);
    int sfIndex = receivedData.indexOf("SF=");
    int bwIndex = receivedData.indexOf("BW=");
    int crIndex = receivedData.indexOf("CR=");

    if (commaIndex1 != -1 && commaIndex2 != -1 && sfIndex != -1 && bwIndex != -1 && crIndex != -1) {
      float temperature = receivedData.substring(0, commaIndex1).toFloat();
      float humidity = receivedData.substring(commaIndex1 + 2, commaIndex2).toFloat();
      int sf = receivedData.substring(sfIndex + 3, bwIndex - 2).toInt();
      float bw = receivedData.substring(bwIndex + 3, crIndex - 2).toFloat();
      int cr = receivedData.substring(crIndex + 3).toInt();

      float snr = LoRa.packetSnr();
      int rssi = LoRa.packetRssi();

      Serial.print("Data diterima dari node ");
      Serial.print(senderAddress);
      Serial.print(": Suhu = ");
      Serial.print(temperature);
      Serial.print(" °C, Kelembaban = ");
      Serial.print(humidity);
      Serial.print("%, SNR = ");
      Serial.print(snr);
      Serial.print(" dB, RSSI = ");
      Serial.print(rssi);
      Serial.print(" dBm");

      // Tambahkan penanda jika data berasal dari repeater
      if (isFromRepeater) {
        Serial.print(", dari Repeater (R1)");
      }

      Serial.println();
    } else {
      Serial.println("Format data tidak valid!");
    }
  }
}