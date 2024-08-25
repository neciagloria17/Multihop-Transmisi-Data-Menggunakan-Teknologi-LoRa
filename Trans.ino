#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define DHTPIN 3          // Pin yang terhubung ke output sensor DHT11
#define DHTTYPE DHT11     // Tipe sensor DHT11
#define SS_PIN 10         // Pin CS (Chip Select) untuk Dragino Shield
#define RST_PIN 9         // Pin RST untuk Dragino Shield
#define BAND 915E6        // Frekuensi LoRa (disesuaikan dengan wilayah Anda)
#define SF 10             // Spreading Factor
#define BW 125E3          // Bandwidth 125 kHz
#define CR 6              // Code Rate 4/6
#define CLIENT_ADDRESS 1  // Alamat pengirim
#define SERVER_ADDRESS 3  // Alamat tujuan (sesuaikan dengan node relay yang dihubungkan)

DHT dht(DHTPIN, DHTTYPE);

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

  dht.begin();
}

void loop() {
  // Baca nilai suhu dan kelembaban dari sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Jika gagal membaca sensor, lanjutkan ke iterasi berikutnya
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca sensor DHT11!");
    delay(2000);
    return;
  }

  // Tampilkan data yang akan dikirim
  Serial.print("Data akan dikirim: ");
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C, Kelembaban: ");
  Serial.print(humidity);
  Serial.println("%");

  // Kirim data menggunakan LoRa
  String data = String(temperature) + " C, " + String(humidity) + " %, SF=" + String(SF) + ", BW=" + String(BW) + ", CR=" + String(CR);  // Tambahkan nilai SF, BW, dan CR ke data yang dikirim
  
  // Kirim data dengan menambahkan informasi alamat pengirim dan alamat tujuan
  LoRa.beginPacket();
  LoRa.write(CLIENT_ADDRESS); // Kirim nilai alamat pengirim langsung
  LoRa.write(SERVER_ADDRESS); // Kirim nilai alamat tujuan langsung
  LoRa.print(data);
  LoRa.endPacket();

  // Tunggu sebelum mengirim data lagi
  delay(5000);
}