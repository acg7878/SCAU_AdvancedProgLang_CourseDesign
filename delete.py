import json

# 文件路径
file_path = '/Users/acg7878/Desktop/MyFile/课设/JWS/data/student.json'

# 读取 JSON 文件
with open(file_path, 'r', encoding='utf-8') as file:
    students = json.load(file)

# 删除所有的 "retake_inf" 字段，并将 studentID 的数值类型改为字符串
for student in students:
    if 'retake_inf' in student:
        del student['retake_inf']
    if 'studentID' in student and isinstance(student['studentID'], int):
        student['studentID'] = str(student['studentID'])
    if 'phone_number' in student and isinstance(student['phone_number'], int):
        student['phone_number'] = str(student['phone_number'])

# 将修改后的数据写回 JSON 文件
with open(file_path, 'w', encoding='utf-8') as file:
    json.dump(students, file, ensure_ascii=False, indent=4)

print("所有的 'retake_inf' 字段已删除，且 'studentID' 字段已转换为字符串。")