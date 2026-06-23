# GPU Task Scheduling Simulator (OS Group Project)

Đây là dự án mô phỏng các thuật toán lập lịch (CPU/GPU Scheduling) trên một tập hợp các tác vụ (tasks) có yêu cầu về thời gian thực thi, bộ nhớ VRAM và độ ưu tiên khác nhau. Đồ án minh họa cách hệ điều hành quản lý tiến trình và cấp phát tài nguyên thực tế.

Chương trình sẽ tự động sinh ngẫu nhiên các tác vụ, mô phỏng quá trình cấp phát tài nguyên (VRAM) và thực thi chúng dựa trên các nguyên tắc của từng thuật toán lập lịch. Sau đó, hệ thống sẽ thực hiện benchmark (đo lường hiệu suất) và xuất kết quả báo cáo ra file CSV để so sánh, đánh giá.

## 🚀 Các thuật toán lập lịch được hỗ trợ
1. **FCFS (First-Come, First-Served):** Đến trước phục vụ trước.
2. **Round Robin (RR):** Lập lịch luân phiên theo lượng tử thời gian (Time Quantum).
3. **Priority Scheduling:** Lập lịch theo độ ưu tiên của tác vụ.
4. **SJF Non-Preemptive (Shortest Job First):** Việc ngắn nhất chạy trước (không độc quyền).
5. **SJF Preemptive (Shortest Remaining Time First):** Việc ngắn nhất chạy trước (có độc quyền - ngắt tiến trình đang chạy nếu có task ngắn hơn xuất hiện).
6. **MLFQ (Multi-Level Feedback Queue):** Hàng đợi đa mức phản hồi.

## 📁 Cấu trúc thư mục
```text
OS_group_project/
├── include/              # Các file tiêu đề (.h) khai báo thư viện, module
├── src/                  # Mã nguồn chính (.c)
│   ├── benchmark/        # Module đánh giá hiệu năng và xuất file CSV
│   ├── gpu/              # Module mô phỏng GPU (VRAM, concurrent tasks)
│   ├── queue/            # Cấu trúc dữ liệu Hàng đợi (Queue)
│   ├── scheduler/        # Mã nguồn triển khai 6 thuật toán lập lịch
│   ├── task/             # Module khởi tạo và quản lý cấu trúc Task
│   └── main.c            # Điểm vào chính của mô phỏng
├── data/                 # Thư mục chứa các file báo cáo benchmark (.csv) sinh ra
├── Makefile              # Kịch bản biên dịch dự án tự động
├── Struct.py             # Script hỗ trợ mô phỏng/testing (nếu có)
└── README.md             # Tài liệu mô tả dự án (file này)
```

## ⚙️ Yêu cầu hệ thống
- Trình biên dịch C (GCC hoặc MinGW trên Windows).
- Cài đặt tiện ích Make (để sử dụng `Makefile`).

## 🛠️ Cách biên dịch và chạy

1. **Biên dịch dự án:**
   Mở Terminal (hoặc Command Prompt/PowerShell) tại thư mục gốc của dự án và chạy:
   ```bash
   make
   ```
   Lệnh này sẽ tự động thu thập mã nguồn và tạo ra file thực thi `gpu_sim` (hoặc `gpu_sim.exe` trên môi trường Windows).

2. **Chạy chương trình:**
   ```bash
   ./gpu_sim
   ```
   *(Trên Windows, bạn có thể gõ trực tiếp `gpu_sim.exe` hoặc click đúp vào file exe).*

3. **Dọn dẹp file biên dịch (Clean):**
   Khi cần xóa file thực thi để build lại từ đầu, chạy:
   ```bash
   make clean
   ```

## 🎮 Hướng dẫn sử dụng
Sau khi khởi chạy, chương trình sẽ yêu cầu bạn nhập vào 2 tham số:
1. **Số lượng Task:** Số tác vụ ngẫu nhiên bạn muốn hệ thống sinh ra để mô phỏng (VD: `10`, `30`, `50`).
2. **Dung lượng VRAM (MB):** Tổng dung lượng bộ nhớ của thiết bị GPU mô phỏng (VD: `4000`, `8000`).

**Luồng hoạt động của hệ thống:**
- Sinh ngẫu nhiên danh sách Task với các thuộc tính: Thời điểm xuất hiện (Arrive), Thời gian tính toán (Compute), Yêu cầu VRAM, và Độ ưu tiên (Priority).
- Thực thi danh sách Task này lần lượt qua cả 6 thuật toán lập lịch với cùng một tài nguyên VRAM đã cấp.
- In trực tiếp bảng thống kê Benchmark (thời gian chờ, thời gian hoàn thành,...) ra Terminal.
- Xuất dữ liệu chi tiết của từng thuật toán vào thư mục `data/` dưới định dạng CSV để bạn dễ dàng mở bằng Excel.
- Phân tích và đưa ra **Đề xuất thuật toán tối ưu nhất** dựa trên số liệu vừa thu thập.

---
**Dự án Nhóm** - Môn học: *Hệ Điều Hành (Operating Systems)*