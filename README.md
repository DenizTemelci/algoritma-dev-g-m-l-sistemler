Gömülü Sistemler (TinyML) İçin Dinamik Tensor Yapısı
Bu proje, bellek kapasitesi kısıtlı olan mikrodenetleyicilerde (Arduino, ESP32 vb.) yapay zeka modellerini verimli bir şekilde çalıştırabilmek amacıyla geliştirilmiş dinamik bir Tensor veri yapısıdır.

Proje Hakkında
Gömülü sistemlerde standart 32-bit (float) dizilerin kullanımı RAM belleği hızla tüketmektedir. Bu projede, belleği optimize etmek amacıyla C dilindeki union yapısı kullanılarak aynı bellek adresinde farklı veri tiplerini (Float32, Float16, Int8) saklayabilen esnek bir yapı kurgulanmıştır.

Temel Özellikler
Dinamik Bellek Yönetimi: malloc ve free fonksiyonları ile matris boyutları çalışma zamanında belirlenir ve bellek yönetimi optimize edilir.

Union Yapısı: Farklı veri tipleri bellekte aynı başlangıç adresini paylaşarak RAM kullanımını minimize eder.

Quantization (Nicemleme): 32-bit ondalıklı veriler, ölçekleme faktörü kullanılarak 8-bit tam sayılara (Int8) dönüştürülür ve bellek kullanımı 4 kat azaltılır.

Agentic Kodlama: Proje geliştirme sürecinde Gemini yapay zeka modeli ile "Agentic Kodlama" yaklaşımı kullanılarak teknik mimari oluşturulmuştur.

Kullanılan Teknolojiler
Dil: C (C89/C99 uyumlu)

IDE: Dev-C++ 5.11

Kavramlar: Struct, Union, Pointer Arithmetic, Dynamic Memory Allocation, Quantization.

Nasıl Çalıştırılır?
main.c dosyasını Dev-C++ ile açın.

Derleyici ayarlarından -std=c99 standardını seçtiğinizden emin olun.

F11 tuşu ile derleyip çalıştırın.
