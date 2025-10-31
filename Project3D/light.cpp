
#include <GL/glut.h>
#include <cmath>

// ================================
// CÁC BIẾN TOÀN CỤC CHO ÁNH SÁNG
// ================================

// Vị trí và trạng thái của các nguồn sáng
bool mainLightOn = true;        // Đèn chính (mặt trời/trần)
bool lampLightOn = true;        // Đèn bàn
bool ambientLightOn = true;     // Ánh sáng môi trường
bool spotLightOn = false;       // Đèn rọi spotlight

float lampIntensity = 1.0f;     // Độ sáng đèn bàn (0.0 -> 1.0)
float timeOfDay = 0.0f;         // Thời gian trong ngày (0-24h) để thay đổi ánh sáng

// Góc và hướng cho spotlight
float spotAngle = 30.0f;        // Góc chiếu của spotlight
float spotDirection[3] = { 0.0f, -1.0f, 0.0f }; // Hướng spotlight (trỏ xuống)

// ================================
// HÀM SETUP ÁNH SÁNG CHÍNH (MẶT TRỜI/ĐÈN TRẦN)
// ================================
void setupMainLight() {
    if (!mainLightOn) {
        glDisable(GL_LIGHT0);
        return;
    }

    glEnable(GL_LIGHT0);

    // Vị trí ánh sáng chính - mô phỏng mặt trời/đèn trần
    // (x, y, z, w) - w=0: ánh sáng hướng (mặt trời), w=1: ánh sáng điểm (đèn)
    GLfloat lightPos[] = { 5.0f, 8.0f, 5.0f, 1.0f };

    // Màu sắc thay đổi theo thời gian trong ngày
    float sunFactor = cosf(timeOfDay * 3.14159f / 12.0f); // Dao động -1 đến 1
    sunFactor = (sunFactor + 1.0f) / 2.0f; // Chuyển về 0-1

    // Ban ngày: ánh sáng trắng vàng, Ban đêm: ánh sáng xanh nhạt
    GLfloat ambient[] = {
        0.2f + sunFactor * 0.3f,  // Red
        0.2f + sunFactor * 0.3f,  // Green  
        0.3f + sunFactor * 0.2f,  // Blue
        1.0f
    };

    GLfloat diffuse[] = {
        0.7f + sunFactor * 0.3f,  // Ánh sáng khuếch tán - tạo độ sáng chính
        0.7f + sunFactor * 0.3f,
        0.7f + sunFactor * 0.2f,
        1.0f
    };

    GLfloat specular[] = {
        1.0f, 1.0f, 1.0f, 1.0f    // Ánh sáng phản chiếu - tạo điểm sáng bóng
    };

    // Áp dụng các thông số ánh sáng
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // Giảm độ sáng theo khoảng cách (attenuation)
    // Công thức: attenuation = 1 / (constant + linear*d + quadratic*d²)
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);   // Hằng số
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);    // Tuyến tính
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f); // Bậc hai
}

// ================================
// ĐÈN BÀN (POINT LIGHT) - ÁNH SÁNG ẤM ÁP
// ================================
void setupLampLight() {
    if (!lampLightOn) {
        glDisable(GL_LIGHT1);
        return;
    }

    glEnable(GL_LIGHT1);

    // Vị trí đèn bàn - điều chỉnh theo vị trí bàn trong scene
    GLfloat lampPos[] = { 2.0f, 3.0f, 2.0f, 1.0f };

    // Ánh sáng vàng ấm áp của đèn bàn
    GLfloat lampAmbient[] = {
        0.3f * lampIntensity,
        0.25f * lampIntensity,
        0.1f * lampIntensity,
        1.0f
    };

    GLfloat lampDiffuse[] = {
        1.0f * lampIntensity,    // Màu vàng cam ấm áp
        0.8f * lampIntensity,
        0.4f * lampIntensity,
        1.0f
    };

    GLfloat lampSpecular[] = {
        0.8f * lampIntensity,
        0.7f * lampIntensity,
        0.5f * lampIntensity,
        1.0f
    };

    glLightfv(GL_LIGHT1, GL_POSITION, lampPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lampAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lampDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lampSpecular);

    // Đèn bàn giảm độ sáng nhanh theo khoảng cách
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.14f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.07f);
}

// ================================
// ĐÈN RỌI (SPOTLIGHT) - CHO TV/BỨC TRANH
// ================================
void setupSpotLight() {
    if (!spotLightOn) {
        glDisable(GL_LIGHT2);
        return;
    }

    glEnable(GL_LIGHT2);

    // Vị trí spotlight (thường ở trên cao, rọi xuống)
    GLfloat spotPos[] = { 0.0f, 5.0f, 0.0f, 1.0f };

    // Ánh sáng trắng sáng của spotlight
    GLfloat spotAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat spotDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat spotSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT2, GL_POSITION, spotPos);
    glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);

    // CÁC THÔNG SỐ SPOTLIGHT
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection); // Hướng chiếu
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spotAngle);         // Góc chiếu (độ)
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0f);           // Độ tập trung (càng cao càng tập trung)

    // Giảm độ sáng
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.09f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.032f);
}

// ================================
// ÁNH SÁNG MÔI TRƯỜNG TOÀN CỤC
// ================================
void setupAmbientLight() {
    if (!ambientLightOn) {
        GLfloat noAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noAmbient);
        return;
    }

    // Ánh sáng môi trường - ánh sáng nền của toàn scene
    // Tạo cảm giác không gian không bị tối đen hoàn toàn
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.25f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // Tính toán ánh sáng cho cả 2 mặt của polygon
    // Hữu ích khi có object mỏng (giấy, vải...)
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

// ================================
// SETUP VẬT LIỆU - QUAN TRỌNG ĐỂ ÁNH SÁNG ĐẸP
// ================================
void setupMaterial(float r, float g, float b, float shininess) {
    // Màu vật liệu khi có ánh sáng môi trường
    GLfloat matAmbient[] = { r * 0.3f, g * 0.3f, b * 0.3f, 1.0f };

    // Màu vật liệu khi có ánh sáng chiếu trực tiếp
    GLfloat matDiffuse[] = { r, g, b, 1.0f };

    // Màu phản chiếu (highlight) - thường là trắng hoặc màu sáng
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Màu phát sáng (emissive) - vật liệu tự phát sáng
    GLfloat matEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // Áp dụng vật liệu
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);

    // Độ bóng (0-128): càng cao càng bóng, điểm sáng càng nhỏ và rõ nét
    // Gỗ: 10-30, Nhựa: 30-60, Kim loại: 80-128
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

// ================================
// VẬT LIỆU MẪU CHO CÁC VẬT THỂ
// ================================

// Vật liệu gỗ (cho bàn, ghế)
void setupWoodMaterial() {
    setupMaterial(0.6f, 0.4f, 0.2f, 25.0f); // Màu nâu gỗ, độ bóng vừa
}

// Vật liệu kim loại (cho chân bàn, đèn)
void setupMetalMaterial() {
    setupMaterial(0.7f, 0.7f, 0.7f, 100.0f); // Màu xám kim loại, rất bóng
}

// Vật liệu nhựa/sơn mịn
void setupPlasticMaterial() {
    setupMaterial(0.8f, 0.8f, 0.8f, 60.0f); // Màu trắng, bóng vừa
}

// Vật liệu kính (cửa sổ)
void setupGlassMaterial() {
    GLfloat matAmbient[] = { 0.2f, 0.3f, 0.4f, 0.6f };   // Trong suốt
    GLfloat matDiffuse[] = { 0.3f, 0.5f, 0.6f, 0.6f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 0.6f };
    GLfloat matEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f); // Rất bóng

    // Bật blending cho độ trong suốt
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Vật liệu phát sáng (bóng đèn)
void setupEmissiveMaterial(float brightness) {
    GLfloat matAmbient[] = { 0.2f, 0.2f, 0.1f, 1.0f };
    GLfloat matDiffuse[] = { 0.8f, 0.7f, 0.3f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 0.8f, 1.0f };

    // Phát sáng - tạo hiệu ứng bóng đèn
    GLfloat matEmission[] = {
        1.0f * brightness,
        0.9f * brightness,
        0.5f * brightness,
        1.0f
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0f);
}

// ================================
// HÀM KHỞI TẠO HỆ THỐNG ÁNH SÁNG
// ================================
void initLighting() {
    // Bật hệ thống ánh sáng OpenGL
    glEnable(GL_LIGHTING);

    // Bật depth test để xử lý đúng các vật che khuất
    glEnable(GL_DEPTH_TEST);

    // Normalize các vector pháp tuyến tự động
    // Quan trọng khi có scale object
    glEnable(GL_NORMALIZE);

    // Bật tính toán màu theo vật liệu
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Setup các nguồn sáng
    setupAmbientLight();
    setupMainLight();
    setupLampLight();
    setupSpotLight();

    // Shading model: GL_SMOOTH cho Gouraud shading (mịn)
    // Hoặc GL_FLAT cho flat shading (góc cạnh)
    glShadeModel(GL_SMOOTH);
}

// ================================
// CẬP NHẬT ÁNH SÁNG THEO THỜI GIAN
// ================================
void updateLighting(float deltaTime) {
    // Cập nhật thời gian trong ngày (0-24h)
    timeOfDay += deltaTime * 0.1f; // Tốc độ thời gian
    if (timeOfDay > 24.0f) timeOfDay = 0.0f;

    // Làm nhấp nháy đèn bàn (hiệu ứng)
    static float lampFlicker = 0.0f;
    lampFlicker += deltaTime * 5.0f;
    lampIntensity = 0.9f + 0.1f * sinf(lampFlicker); // Dao động 0.9-1.0

    // Cập nhật lại các nguồn sáng
    setupMainLight();
    setupLampLight();
}

// ================================
// HÀM BẬT/TẮT ÁNH SÁNG (GỌI TỪ KEYBOARD.CPP)
// ================================
void toggleMainLight() {
    mainLightOn = !mainLightOn;
    setupMainLight();
}

void toggleLampLight() {
    lampLightOn = !lampLightOn;
    setupLampLight();
}

void toggleSpotLight() {
    spotLightOn = !spotLightOn;
    setupSpotLight();
}

void toggleAmbientLight() {
    ambientLightOn = !ambientLightOn;
    setupAmbientLight();
}

// Thay đổi cường độ đèn bàn
void adjustLampIntensity(float delta) {
    lampIntensity += delta;
    if (lampIntensity < 0.0f) lampIntensity = 0.0f;
    if (lampIntensity > 1.0f) lampIntensity = 1.0f;
    setupLampLight();
}

// Thay đổi thời gian trong ngày
void setTimeOfDay(float hour) {
    timeOfDay = hour;
    setupMainLight();
}