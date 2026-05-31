# LPTV Password Manager

Một ứng dụng quản lý mật khẩu dòng lệnh (CLI) mạnh mẽ, bảo mật và dễ sử dụng, được phát triển bằng C++17. LPTV (Local Password Tiny Vault) giúp bạn lưu trữ và quản lý thông tin tài khoản một cách an toàn trên máy cục bộ.

## 🌟 Tính năng chính

- **Bảo mật tốt:** Sử dụng các thuật toán
    - **Argon2** để bảo vệ Master Password
    - **AES-256-GCM** để bảo vệ dữ liệu người dùng.
- **Cơ chế Session:** Tự động khóa vault sau 5 phút không hoạt động để đảm bảo an toàn.
- **Quản lý linh hoạt:**
    - Lưu trữ tài khoản theo Service (tên dịch vụ).
    - Phân loại tài khoản theo Category (danh mục).
    - Tìm kiếm nhanh chóng theo tên dịch vụ hoặc tên người dùng.
- **Giao diện dòng lệnh chuyên nghiệp:** Hỗ trợ màu sắc, nhập mật khẩu ẩn và chế độ Interactive Shell tiện lợi.
- **Lưu trữ dữ liệu:** Dữ liệu được tổ chức dưới định dạng JSON, dễ dàng sao lưu và bảo trì.

## 🛠 Công nghệ sử dụng

- **Ngôn ngữ:** C++17
- **Hệ điều hành:** Windows (win32)
- **Thư viện:**
    - [Argon2](https://github.com/P-H-C/phc-winner-argon2): Băm và xác thực mật khẩu.
    - [OpenSSL](https://www.openssl.org/): Cung cấp các thuật toán mã hóa AES-256-GCM và trình tạo số ngẫu nhiên an toàn.
    - [nlohmann/json](https://github.com/nlohmann/json): Xử lý định dạng dữ liệu JSON.
- **Build System:** CMake

## 🚀 Cài đặt và Sử dụng

### Yêu cầu hệ thống
- Trình biên dịch hỗ trợ C++17 (MSVC, GCC, hoặc Clang).
- CMake 3.10 trở lên.
- Thư viện `libargon2`
- Thư viện `OpenSSL`
- Thư viện `nlohmann/json`

### Hướng dẫn Build
```bash
# Tạo thư mục build
mkdir build
cd build

# Cấu hình project
cmake .. -G "MinGW Makefiles"

# Biên dịch
mingw32-make
```
Sau khi biên dịch thành công, bạn sẽ nhận được tệp thực thi `lptv.exe`.

### Các lệnh cơ bản

| Lệnh | Mô tả |
| :--- | :--- |
| `lptv init` | Khởi tạo Vault mới và thiết lập Master Password. |
| `lptv unlock` | Mở khóa Vault để bắt đầu sử dụng. |
| `lptv lock` | Khóa Vault thủ công và xóa session hiện tại. |
| `lptv status` | Kiểm tra trạng thái Vault (đã khởi tạo/mở khóa chưa). |
| `lptv list` | Liệt kê tất cả tài khoản hoặc danh mục. |
| `lptv add` | Thêm mới một tài khoản hoặc danh mục. |
| `lptv get` | Xem chi tiết tài khoản và copy mật khẩu vào clipboard. |
| `lptv update` | Cập nhật thông tin tài khoản hiện có. |
| `lptv delete` | Xóa tài khoản hoặc danh mục. |
| `lptv search` | Tìm kiếm tài khoản theo từ khóa. |
| `lptv shell` | Vào chế độ Interactive Shell (không cần gõ tiền tố `lptv`). |

> **Lưu ý:**
> - **Chế độ độc lập (Standalone):** Khi chạy các lệnh như `list`, `add`, `get`,... nếu Vault đang khóa, ứng dụng sẽ **tự động yêu cầu nhập mật khẩu** để thực thi lệnh ngay lập tức.
> - **Chế độ Shell:** Trong `lptv shell`, bạn cần dùng lệnh `unlock` để mở khóa một lần và sử dụng cho cả phiên làm việc.

*Dùng `lptv help <command>` để xem hướng dẫn chi tiết cho từng lệnh.*

## 📂 Cấu trúc thư mục

```text
.
├── include/            # Các tệp tiêu đề (.h)
│   ├── cli/            # Xử lý giao diện dòng lệnh và parser
│   ├── core/           # Logic nghiệp vụ chính (Vault, Session, Manager)
│   ├── storage/        # Xử lý lưu trữ
│   └── utils/          # Các tiện ích bổ trợ (Security, Constants) và mã hóa dữ liệu
├── src/                # Các tệp thực thi (.cpp)
├── build/              # Thư mục chứa tệp sau khi biên dịch
└── CMakeLists.txt      # Cấu hình build project
```

## 🔐 Cơ chế bảo mật

Ứng dụng được thiết kế với các tiêu chuẩn bảo mật hiện đại để bảo vệ dữ liệu của bạn:

1. **Mã hóa dữ liệu (Encryption):**
   - Sử dụng thuật toán **AES-256-GCM** (Galois/Counter Mode), tiêu chuẩn mã hóa đối xứng mạnh nhất hiện nay.
   - Cơ chế GCM không chỉ mã hóa dữ liệu mà còn đảm bảo tính toàn vẹn (Integrity), giúp phát hiện bất kỳ sự thay đổi trái phép nào trên tệp dữ liệu.
   - Mỗi lần lưu dữ liệu, một **Nonce (Initialization Vector)** ngẫu nhiên 12-byte và một **Auth Tag** 16-byte được tạo ra để đảm bảo tính duy nhất.

2. **Dẫn xuất khóa (Key Derivation):**
   - Master Password không được dùng trực tiếp làm khóa mã hóa.
   - Sử dụng **Argon2id** (phiên bản chống lại các cuộc tấn công side-channel và tối ưu cho GPU/ASIC) để tạo ra khóa 256-bit từ mật khẩu của bạn.
   - Thông tin xác thực mật khẩu được lưu dưới dạng băm **SHA-256** kèm theo muối (salt) ngẫu nhiên trong `.lptv/lptv.dat`.

3. **Quản lý phiên (Session Management):**
   - **Xác thực 2 lớp:** Hệ thống kiểm tra cả tệp timestamp trên đĩa (`session.dat`) và khóa phiên trong bộ nhớ để cấp quyền.
   - **Tự động hết hạn:** Phiên làm việc (Session) sẽ tự động vô hiệu hóa sau **300 giây (5 phút)** không hoạt động.
   - **Xóa sạch bộ nhớ:** Các dữ liệu nhạy cảm (như Master Password khi nhập) được xóa khỏi RAM ngay sau khi sử dụng để tránh tấn công memory dumping.

4. **Lưu trữ an toàn:**
   - Mọi dữ liệu nhạy cảm được lưu trong thư mục ẩn `.lptv/`.
   - Cơ sở dữ liệu `vault.db` luôn được mã hóa hoàn toàn, chỉ giải mã khi có Master Password đúng.

