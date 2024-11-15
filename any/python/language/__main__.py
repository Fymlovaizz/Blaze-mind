import json
import random
import os

# Input    
languages = ['English', 'Russian', 'Finnish', 'German', 'Chinese', 'Japanese']

info_fields = ['word', 'pronunciation', 'type', 'meaning', "explanation", "explanation-vietnamese", "example"]
unneeded_info = ['example']
displayed_info_count = random.randint(2, 3)
required_info_count = 5 - displayed_info_count - 1

info_fields = [field for field in info_fields if field not in unneeded_info]

# Module 1: Xử lý tệp dữ liệu từ vựng
def load_vocabulary_data(file_path):
    if not os.path.isfile(file_path):
        print(f"File {file_path} không tồn tại! Tạo file rỗng.")
        create_empty_vocabulary_file(file_path)
        return []

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            return json.load(file)
    except json.JSONDecodeError:
        print(f"Lỗi định dạng JSON trong {file_path}! Tạo file rỗng.")
        input("Nhấn Enter để tiếp tục...")
        create_empty_vocabulary_file(file_path)
        return []

def create_empty_vocabulary_file(file_path):
    with open(file_path, 'w', encoding='utf-8') as file:
        json.dump([], file)
    print(f"Tạo file từ vựng rỗng: {file_path}")
    input("Nhấn Enter để tiếp tục...")

# Module 2: Tạo câu hỏi
def generate_question(vocabulary_list):
    print("\033c", end="")

    # Generate question
    if not vocabulary_list:
        print("Không có từ vựng trong ngôn ngữ này.")
        input("Nhấn Enter để tiếp tục...")
        return None

    # Select random word
    word_data = random.choice(vocabulary_list)
    options = [word_data]
    while len(options) < 5 and len(options) < len(vocabulary_list):
        random_word = random.choice(vocabulary_list)
        if random_word not in options:
            options.append(random_word)
    random.shuffle(options)
    displayed_fields = random.sample(info_fields, displayed_info_count)

    # Print question
    print("Dựa trên thông tin sau")
    for field in displayed_fields:
        print(f"\t{field.capitalize()}: {word_data[field]}")

    answer_fields = [field for field in info_fields if field not in displayed_fields]
    print(f"Chọn {', '.join(answer_fields[i] for i in range(required_info_count))} đúng!")
    for i, option in enumerate(options):
        print(f"\t{i+1}. {', '.join(option[answer_fields[i]] for i in range(required_info_count))}")

    # Process user's choice
    try:
        user_choice = input("\nLựa chọn của bạn (0 để thoát): ")
        if user_choice == '0':
            return False
        elif options[int(user_choice) - 1] == word_data:
            print("Chính xác! Thông tin thêm:")
        else:
            print(f"Sai! Đáp án đúng là:")

        for idx, field in enumerate(info_fields):
            print(f"\t{field.capitalize()}: {word_data[field]}")
        input("\nNhấn Enter để tiếp tục...")
    except:
        print("Lựa chọn không hợp lệ, thử bạn.")
        input("Nhấn Enter để tiếp tục...")

    return True

# Module 3: Menu ngôn ngữ
def select_language(languages):
    while True:
        print("\033c", end="")
        print("Chọn ngôn ngữ bạn muốn học hoặc nhấn 0 để thoát:")
        for i, language in enumerate(languages):
            print(f"{i+1}. {language}")

        user_input = input("Nhập lựa chọn của bạn: ")

        if user_input == '0':
            print("Thoát chương trình.")
            break

        if user_input.isdigit() and 1 <= int(user_input) <= len(languages):
            chosen_language = languages[int(user_input) - 1]
            vocabulary_list = load_vocabulary_data(f"coding/python/language/{chosen_language}.json")

            while True:
                if not generate_question(vocabulary_list):
                    break
        else:
            print("Lựa chọn không hợp lệ, thử lại.")
            input("Nhấn Enter để tiếp tục...")

# Module 4: Khởi động chương trình
def start_program():
    select_language(languages)

if __name__ == "__main__":
    start_program()
