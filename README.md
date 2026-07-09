# LPTV Password Manager

Một ứng dụng quản lý mật khẩu dòng lệnh (CLI) hiệu năng cao, bảo mật và dễ sử dụng, được phát triển bằng **C++17**. LPTV giúp bạn lưu trữ và quản lý thông tin tài khoản cục bộ một cách an toàn.

## 🌟 Tính năng nổi bật

*   **Bảo mật cao:**
    *   **Argon2id:** Thuật toán băm Master Password chống lại các cuộc tấn công brute-force phần cứng tối ưu nhất hiện nay.
    *   **AES-256-GCM:** Mã hóa đối xứng đảm bảo cả tính bảo mật và tính toàn vẹn của dữ liệu.
*   **Quản lý thông minh & linh hoạt:**
    *   Phân loại tài khoản theo danh mục (Category).
    *   Tìm kiếm nhanh chóng theo tên dịch vụ, tên đăng nhập hoặc danh mục.
*   **Trình tạo mật khẩu ngẫu nhiên:** Hỗ trợ tạo mật khẩu mạnh với độ dài tùy chọn và tùy biến ký tự.
*   **Interactive Shell tiện lợi:** Chế độ dòng lệnh tương tác không cần gõ lại tiền tố lệnh, tự động đóng phiên (tự thoát chương trình) sau **1 phút** để tránh rò rỉ dữ liệu khi người dùng rời đi.
*   **Tự động xóa Clipboard:** Sao chép mật khẩu vào bộ nhớ tạm thời và tự động xóa sau **30 giây** để tránh việc phần mềm độc hại đọc trộm.
*   **Lưu trữ nhị phân an toàn:** Toàn bộ dữ liệu được lưu trữ dưới dạng nhị phân mã hóa trong thư mục ẩn `.lptv/`, đảm bảo không ai có thể đọc trộm file nếu không có Master Password.

## 🛠 Công nghệ sử dụng

*   **Ngôn ngữ lập trình:** C++17
*   **Hệ điều hành tối ưu:** Windows (Win32 API)
*   **Công cụ Build:** CMake (3.10+)
*   **Thư viện bảo mật cốt lõi:**
    *   **OpenSSL:** Cung cấp giải thuật mã hóa AES-256-GCM và sinh số ngẫu nhiên bảo mật (CSPRNG).
    *   **Argon2:** Hàm dẫn xuất khóa (KDF) mạnh mẽ bảo vệ Master Password.

## 🚀 Hướng dẫn Cài đặt & Biên dịch

### Yêu cầu tiên quyết
Để build ứng dụng, máy tính của bạn cần cài đặt sẵn:
1. Trình biên dịch hỗ trợ C++17 (ví dụ: MinGW-w64, GCC, hoặc Clang).
2. CMake 3.10 trở lên.
3. Thư viện `libargon2` và `OpenSSL`.

### Các bước Build dự án (trên Windows dùng MinGW)

Trước tiên, bạn cần clone thư viện `secure-string` vào thư mục `external/Secure-string`:
```bash
# Clone thư viện secure-string vào thư mục external
git clone "https://github.com/VuLeCoder/Secure-string" external/Secure-string
```

Sau đó, mở PowerShell/CMD tại thư mục dự án và chạy các lệnh sau để build:
```bash
# 1. Tạo và di chuyển vào thư mục build
mkdir build
cd build

# 2. Cấu hình dự án bằng CMake
cmake .. -G "MinGW Makefiles"

# 3. Biên dịch mã nguồn
mingw32-make
```
Sau khi hoàn thành, file thực thi **`lptv.exe`** sẽ được tạo ra trong thư mục `build`. Bạn có thể thêm đường dẫn của file này vào biến môi trường PATH để gọi lệnh `lptv` từ bất kỳ đâu.

## 💡 Các lệnh nổi bật & Cách sử dụng

### 1. `lptv init` (Khởi tạo Vault)
Khởi tạo cơ sở dữ liệu mật khẩu mới và thiết lập Master Password để bảo vệ toàn bộ dữ liệu.
*   *Cách dùng:* `lptv init`

### 2. `lptv shell` (Interactive Shell - Khuyên dùng)
Vào chế độ shell tương tác giúp thực thi các lệnh tiếp theo cực nhanh mà không cần gõ tiền tố `lptv`. 
> 🔒 **Bảo mật:** Shell sẽ tự động kết thúc phiên làm việc và đóng chương trình sau **5 phút** kể từ lúc mở.
*   *Cách dùng:* `lptv shell`

### 3. `lptv add` (Thêm tài khoản mới)
Thêm một tài khoản đăng nhập cho một dịch vụ. Chương trình sẽ nhắc bạn nhập Username, Password, Danh mục và Ghi chú một cách bảo mật.
*   *Cách dùng:* `lptv add <tên_dịch_vụ>`
*   *Thêm danh mục mới:* `lptv add --category <tên_danh_mục>`

### 4. `lptv get` (Truy xuất mật khẩu & Copy nhanh)
Xem thông tin chi tiết tài khoản của một dịch vụ. 
*   *Xem mật khẩu ẩn (mặc định):* `lptv get <tên_dịch_vụ>`
*   *Hiện mật khẩu dạng text:* `lptv get <tên_dịch_vụ> --show`
*   *Sao chép nhanh:* Chương trình sẽ hỏi bạn có muốn copy mật khẩu vào Clipboard hay không. Nếu đồng ý, mật khẩu sẽ tự động bị xóa khỏi bộ nhớ tạm sau **30 giây**.

### 5. `lptv search` (Tìm kiếm thông minh)
Tìm kiếm tài khoản dựa trên từ khóa khớp với tên dịch vụ, tên đăng nhập hoặc danh mục.
*   *Cách dùng:* `lptv search <từ_khóa>`

### 6. `lptv generate` (Tạo mật khẩu cực mạnh)
Tự động sinh một chuỗi mật khẩu ngẫu nhiên, an toàn với độ dài và các bộ ký tự tùy biến.
*   *Tạo mật khẩu 20 ký tự:* `lptv generate 20`
*   *Tạo mật khẩu không bao gồm ký tự đặc biệt:* `lptv generate 16 --no-special`

---

## 📂 Cấu trúc thư mục dự án

```text
.
├── include/            # Các tệp tiêu đề (.h)
│   ├── cli/            # Xử lý Console, Parser lệnh và Dispatcher
│   ├── core/           # Logic nghiệp vụ chính (PasswordManager, Session, Vault)
│   ├── storage/        # Đọc/Ghi dữ liệu dưới định dạng nhị phân bảo mật
│   └── utils/          # Các tiện ích (mã hóa, sinh số ngẫu nhiên, hằng số)
├── src/                # File mã nguồn (.cpp) tương ứng
├── external/           # Thư viện ngoài (SecureString cấp phát bộ nhớ an toàn)
└── CMakeLists.txt      # File cấu hình build CMake
```
