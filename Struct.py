import random

class Task:
    def __init__(self, task_id):
        self.id = task_id
        # Sinh ngẫu nhiên các thuộc tính như trong sơ đồ của bạn
        self.arrival_time = random.randint(0, 20)      # Thời điểm task đến hệ thống
        self.priority = random.randint(1, 5)           # Độ ưu tiên (1 là cao nhất)
        self.data_size = random.randint(100, 4000)     # Dung lượng dữ liệu (MB)
        self.burst_time = random.randint(5, 50)        # Thời gian cần thiết để chạy xong (ms)

    def __str__(self):
        # Định dạng chuỗi in ra cho đẹp và dễ nhìn
        return (f"Task_{self.id:02d} | "
                f"Arrive: {self.arrival_time:02d}ms | "
                f"Pri: {self.priority} | "
                f"Size: {self.data_size:4d}MB | "
                f"Burst: {self.burst_time:02d}ms")

def generate_tasks(num_tasks=20):
    """Hàm sinh ngẫu nhiên danh sách task"""
    tasks = []
    for i in range(1, num_tasks + 1):
        tasks.append(Task(task_id=i))
    return tasks

def print_tasks(tasks):
    """Hàm in danh sách task ra terminal để kiểm tra"""
    print("-" * 65)
    print(f"PHASE 1: ĐÃ SINH NGẪU NHIÊN {len(tasks)} TASK")
    print("-" * 65)
    for task in tasks:
        print(task)
    print("-" * 65)

# ==========================================
# CHẠY THỬ PHASE 1
# ==========================================
if __name__ == "__main__":
    # Khởi tạo 20 task ngẫu nhiên
    job_queue = generate_tasks(20)
    
    # In ra terminal
    print_tasks(job_queue)