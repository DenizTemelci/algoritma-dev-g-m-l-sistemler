#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// 1. Adým: Tensor içindeki verinin tipini belirlemek için Enum
typedef enum {
    TENSOR_FLOAT32, // 32-bit ondalýklý
    TENSOR_FLOAT16, // 16-bit ondalýklý (C'de standart olmadýðý için uint16_t ile simüle edeceðiz)
    TENSOR_INT8     // 8-bit tam sayý (Quantized)
} TensorType;

// 2. Adým: Dinamik Tensor Yapýsý ve UNION kullanýmý
typedef struct {
    int rows;
    int cols;
    TensorType type;
    
    // UNION: Bellekte ayný adresi paylaþan deðiþkenler.
    // O an hangi tipte veri saklýyorsak sadece onun yerini kaplar, bellek tasarrufu saðlar.
    union {
        float *f32_data;
        uint16_t *f16_data;
        int8_t *i8_data;
    } data;
} Tensor;

// Tensor oluþturma (Matris Yönetimi ve Dinamik Bellek)
Tensor* create_tensor(int rows, int cols, TensorType type) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    int num_elements = rows * cols;
    
    t->rows = rows;
    t->cols = cols;
    t->type = type;

    // Tipe göre bellek tahsisi (Memory Allocation)
    if (type == TENSOR_FLOAT32) {
        t->data.f32_data = (float*)malloc(num_elements * sizeof(float));
    } else if (type == TENSOR_FLOAT16) {
        t->data.f16_data = (uint16_t*)malloc(num_elements * sizeof(uint16_t));
    } else if (type == TENSOR_INT8) {
        t->data.i8_data = (int8_t*)malloc(num_elements * sizeof(int8_t));
    }
    return t;
}

// Belleði temizleme
void free_tensor(Tensor* t) {
    if (t->type == TENSOR_FLOAT32) free(t->data.f32_data);
    else if (t->type == TENSOR_FLOAT16) free(t->data.f16_data);
    else if (t->type == TENSOR_INT8) free(t->data.i8_data);
    free(t);
}

// 3. Adým: Quantization (Float32'den Int8'e dönüþtürme demosu)
// Büyük boyutlu ondalýklý sayýlarý, belli bir oranda küçülterek 8-bit tam sayýya çeviririz.
Tensor* quantize_f32_to_i8(Tensor* f32_tensor) {
    int num_elements;
    Tensor* i8_tensor;
    float max_val = 0.0f;
    float scale;
    int i; // C89 standardý için döngü deðiþkenini en baþta tanýmlýyoruz

    if (f32_tensor->type != TENSOR_FLOAT32) return NULL;

    num_elements = f32_tensor->rows * f32_tensor->cols;
    i8_tensor = create_tensor(f32_tensor->rows, f32_tensor->cols, TENSOR_INT8);

    // Basit bir simetrik quantization için maksimum mutlak deðeri bulalým
    for (i = 0; i < num_elements; i++) {
        if (fabs(f32_tensor->data.f32_data[i]) > max_val) {
            max_val = fabs(f32_tensor->data.f32_data[i]);
        }
    }

    // Scale faktörü (Verileri -127 ile 127 arasýna sýðdýrmak için)
    scale = max_val / 127.0f;

    for (i = 0; i < num_elements; i++) {
        // Tip Dönüþümü (Type Casting): float veriyi int8_t tipine çeviriyoruz
        i8_tensor->data.i8_data[i] = (int8_t)round(f32_tensor->data.f32_data[i] / scale);
    }

    printf("\n[Sistem] Quantization Tamamlandi. Scale Factor: %f\n", scale);
    return i8_tensor;
}

// Tensoru ekrana yazdýrma
void print_tensor(Tensor* t) {
    int num_elements = t->rows * t->cols;
    int i; // C89 standardý için döngü deðiþkenini en baþta tanýmlýyoruz

    if (t->type == TENSOR_FLOAT32) {
        printf("Float32 Tensor (%dx%d):\n", t->rows, t->cols);
        for (i = 0; i < num_elements; i++) {
            printf("%f ", t->data.f32_data[i]);
            if ((i + 1) % t->cols == 0) printf("\n");
        }
    } else if (t->type == TENSOR_INT8) {
        printf("Int8 (Quantized) Tensor (%dx%d):\n", t->rows, t->cols);
        for (i = 0; i < num_elements; i++) {
            printf("%d ", t->data.i8_data[i]); // Sadece tam sayý yazdýrýr
            if ((i + 1) % t->cols == 0) printf("\n");
        }
    }
}

int main() {
    Tensor* my_model_weights;
    Tensor* quantized_weights;

    // 1. Orijinal 32-bit Float Matris (Tensor) oluþturulmasý
    my_model_weights = create_tensor(2, 2, TENSOR_FLOAT32);
    
    // Matrise örnek aðýrlýk deðerleri atayalým
    my_model_weights->data.f32_data[0] = 12.5f;
    my_model_weights->data.f32_data[1] = -5.3f;
    my_model_weights->data.f32_data[2] = 0.8f;
    my_model_weights->data.f32_data[3] = 25.1f;

    printf("--- ORJINAL YAPAY ZEKA MODEL AGIRLIKLARI ---\n");
    print_tensor(my_model_weights);

    // 2. Quantization Ýþlemi Uygulanmasý
    quantized_weights = quantize_f32_to_i8(my_model_weights);

    printf("\n--- QUANTIZE EDILMIS (SIKISTIRILMIS) AGIRLIKLAR ---\n");
    print_tensor(quantized_weights);

    // Bellek Temizliði
    free_tensor(my_model_weights);
    free_tensor(quantized_weights);

    return 0;
}
