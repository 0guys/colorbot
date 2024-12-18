# ColorBot3.2

## 📦 目錄

- [簡介](#簡介)
- [功能](#功能)
- [硬體需求](#硬體需求)
- [軟體需求](#軟體需求)
- [安裝步驟](#安裝步驟)
- [連接說明](#連接說明)
- [使用說明](#使用說明)
- [版本歷史](#版本歷史)
- [貢獻](#貢獻)
- [許可證](#許可證)
- [聯絡資訊](#聯絡資訊)

## 簡介

**ColorBot3.2.1** 是一款基於 ESP32 的自主避障小車，配備了多個超聲波感測器和顏色識別功能。這款小車能夠在偵測到障礙物時自動轉向，並根據顏色感測器的讀取值執行不同的動作，如前進、後退或伺服馬達旋轉。

## 功能

- **自主避障**：使用前方、左側和右側的超聲波感測器檢測障礙物，並自動選擇轉向方向。
- **顏色識別控制**：
  - **紅色**：小車後退。
  - **藍色**：小車前進。
  - **綠色**：控制伺服馬達旋轉。
- **非阻塞式轉向控制**：使用 `millis()` 函數實現非阻塞式的轉向，提升系統反應速度和多任務處理能力。
- **手動控制模式**：通過控制器（如 PS4 控制器）手動控制小車的運動。
- **LED 指示燈**：指示當前運行模式（自動或手動）。

## 硬體需求

- **微控制器**：
  - ESP32 開發板（如 ESP32 DevKitC）
- **馬達控制**：
  - L298N 馬達驅動模組
  - 直流馬達 2 類
- **感測器**：
  - 超聲波感測器（前方、左側、右側各一）
  - 顏色感測器（如 TCS3200）
- **其他**：
  - 伺服馬達（用於顏色識別動作）
  - 電源供應（如 12V 電池）
  - 連接線
  - 面包板（如需要）

## 軟體需求

- **Arduino IDE**（建議使用最新版本）
- **所需函式庫**：
  - [Bluepad32](https://github.com/Bluepad32/Bluepad32)（用於控制器連接）
  - [ESP32Servo](https://github.com/kroimon/ESP32Servo)（用於伺服馬達控制）
  - 其他依賴函式庫根據專案需求安裝

## 安裝步驟

1. **下載專案**：
   - 直接從 [GitHub](https://github.com/0guys/colorbot) 下載 ZIP 檔案，或使用 Git 克隆專案：
     ```bash
     git clone https://github.com/yourusername/ColorBot3.2.1.git
     ```

2. **安裝 Arduino IDE**：
   - 下載並安裝 [Arduino IDE](https://www.arduino.cc/en/software)。

3. **安裝必要的函式庫**：
   - 打開 Arduino IDE，前往 **工具 > 管理庫**，搜索並安裝以下函式庫：
     - **Bluepad32**
     - **ESP32Servo**
     - 其他相關函式庫如 `Servo`（通常已內建）

4. **配置 ESP32 開發板**：
   - 在 Arduino IDE 中，前往 **工具 > 開發板 > 開發板管理員**，搜索並安裝 **ESP32**。
   - 選擇正確的 ESP32 開發板（如 ESP32 DevKitC）：
     - **工具 > 開發板** 選擇 `ESP32 Dev Module` 或對應您的開發板型號。
   - 前往 **工具 > 端口** 選擇正確的串口。

5. **上傳程式碼**：
   - 打開 `colorbot3.2.1.ino` 檔案。
   - 點擊 **上傳** 按鈕將程式碼上傳至 ESP32 開發板。

## 連接說明

### 馬達連接

- **L298N 馬達驅動模組**：
  - IN1 → ESP32 GPIO 16
  - IN2 → ESP32 GPIO 17
  - IN3 → ESP32 GPIO 18
  - IN4 → ESP32 GPIO 19
  - ENA → ESP32 GPIO 21 (PWM)
  - ENB → ESP32 GPIO 22 (PWM)
  - VCC 和 GND 分別連接至電源和地線

### 超聲波感測器連接

- **前方感測器**：
  - Trig → ESP32 GPIO 23
  - Echo → ESP32 GPIO 25

- **左側感測器**：
  - Trig → ESP32 GPIO 26
  - Echo → ESP32 GPIO 27

- **右側感測器**：
  - Trig → ESP32 GPIO 32
  - Echo → ESP32 GPIO 33

### 顏色感測器連接

- **TCS3200 顏色感測器**：
  - VCC → ESP32 3.3V
  - GND → ESP32 GND
  - OUT → ESP32 GPIO 34
  - S0 → ESP32 GPIO 14
  - S1 → ESP32 GPIO 12
  - S2 → ESP32 GPIO 13
  - S3 → ESP32 GPIO 15

### 伺服馬達連接

- **伺服馬達**：
  - 信號線 → ESP32 GPIO 4
  - 電源和地線分別連接至電源和地線

### 控制器連接

- 通過 Bluetooth 將控制器（如 PS4 控制器）與 ESP32 連接，確保已正確配置 Bluepad32 庫。

## 使用說明

1. **啟動小車**：
   - 確保所有硬體連接正確，並接通電源。

2. **運行程式碼**：
   - 確認 Arduino IDE 已上傳最新的 `colorbot3.2.1.ino` 程式碼至 ESP32。

3. **模式切換**：
   - 使用控制器上的 Share 按鈕切換自動模式和手動模式。
   - **自動模式**：小車將根據感測器數據自動避障和執行顏色識別動作。
   - **手動模式**：通過控制器的搖桿手動控制小車的前進、後退、左轉和右轉。

4. **顏色識別**：
   - **紅色**：小車後退。
   - **藍色**：小車前進。
   - **綠色**：伺服馬達旋轉至180度並回到0度。

5. **避障功能**：
   - 當前方距離小於10公分時，小車會根據左右側距離決定轉向方向。
   - 當左右兩側距離都大於380公分時，小車將自動向右轉。

## 版本歷史

### 版本 3.2.1 - 2024年4月27日

**新增功能**
- **新增避障條件**：當左右兩側的距離都大於380公分時，小車將自動向右轉，確保在開放空間中更靈活的導航。
- **實現非阻塞式轉向控制**：使用 `millis()` 函數管理轉向持續時間，取代傳統的阻塞式 `delay()`，提升系統反應速度和多任務處理能力。

**修正問題**
- **修正馬達控制相關錯誤**：解決編譯時未聲明 `CHANNEL_ENA`、`CHANNEL_ENB`、`CHANNEL_ENA2`、`CHANNEL_ENB2` 及 `setMotorPWM` 函數的錯誤。將馬達控制函數集中於 `MotorControl.h` 和 `MotorControl.cpp` 中，並在主程式中正確引用，消除了作用域錯誤。
- **修正控制器按鍵判斷錯誤**：將控制器按鍵判斷從 `isButtonPressed(PS4_BUTTON_SHARE)` 修正為 `isKeyPressed(KEY_SELECT)`，並根據 Bluepad32 庫的正確按鍵常數進行調整，解決了控制器按鍵未定義的錯誤。

**優化**
- **程式碼模組化**：將馬達控制、感測器讀取等功能模組化，提升代碼的可讀性和可維護性，便於未來的功能擴展和維護。
- **增強除錯資訊**：在關鍵步驟和狀態轉換處添加更多的 `Serial.println` 語句，便於開發和除錯過程中追蹤系統狀態，提升開發效率。

## 貢獻

歡迎各位開發者和愛好者參與本專案！請遵循以下步驟進行貢獻：

1. **Fork 專案**
2. **建立新分支**
   ```bash
   git checkout -b feature/your-feature-name
