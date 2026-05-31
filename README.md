# LPTV Password Manager

Một ứng dụng quản lý mật khẩu dòng lệnh (CLI) mạnh mẽ, bảo mật và dễ sử dụng, được phát triển bằng C++17. LPTV (Local Password Tiny Vault) giúp bạn lưu trữ và quản lý thông tin tài khoản một cách an toàn trên máy cục bộ.

## 🌟 Tính năng chính

- **Bảo mật tốt:** Sử dụng thuật toán băm **Argon2** (thắng giải Password Hashing Competition) để bảo vệ Master Password.
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
    - [nlohmann/json](https://github.com/nlohmann/json): Xử lý định dạng dữ liệu JSON.
- **Build System:** CMake

## 🚀 Cài đặt và Sử dụng

### Yêu cầu hệ thống
- Trình biên dịch hỗ trợ C++17 (MSVC, GCC, hoặc Clang).
- CMake 3.10 trở lên.
- Thư viện `libargon2` đã được cài đặt trên hệ thống.

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

*Dùng `lptv help <command>` để xem hướng dẫn chi tiết cho từng lệnh.*

## 📂 Cấu trúc thư mục

```text
.
├── include/            # Các tệp tiêu đề (.h)
│   ├── cli/            # Xử lý giao diện dòng lệnh và parser
│   ├── core/           # Logic nghiệp vụ chính (Vault, Session, Manager)
│   ├── storage/        # Xử lý lưu trữ và mã hóa dữ liệu
│   └── utils/          # Các tiện ích bổ trợ (Security, Constants)
├── src/                # Các tệp thực thi (.cpp)
├── build/              # Thư mục chứa tệp sau khi biên dịch
└── CMakeLists.txt      # Cấu hình build project
```

## 🔐 Cơ chế bảo mật

1. **Master Password:** Không được lưu trực tiếp. Chỉ lưu giá trị băm (hash) sử dụng Argon2 với muối (salt) ngẫu nhiên.
2. **Vault Data:** Dữ liệu tài khoản được lưu trữ trong `vault.db`.
3. **Session Management:** Khi `unlock`, một tệp session tạm thời được tạo ra với timestamp. Các lệnh yêu cầu quyền truy cập sẽ kiểm tra tính hợp lệ của session này.

---
*Dự án đang trong quá trình phát triển, cụ thể là dữ liệu tài khoản người dùng đang lưu bản rõ.*
