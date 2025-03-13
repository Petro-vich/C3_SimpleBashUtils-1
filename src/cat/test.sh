#!/bin/bash

success=0

run_test() {
    local flag=$1
    local file=$2

    original_output=$(cat $flag $file)

    your_output=$(./s21_cat $flag $file)

    if [ "$original_output" == "$your_output" ]; then
        echo "Тест с флагом $flag и файлом $file: УСПЕШНО"
        ((success++))
    else
        echo "Тест с флагом $flag и файлом $file: ОШИБКА"
        echo "Ожидаемый вывод (cat):"
        echo "$original_output"
        echo "Ваш вывод:"
        echo "$your_output"
    fi
}

flags=("-T" "-b" "--number-nonblank" "-n" "--number" "-t" "-v" "-s" "--squeeze-blank")

files=("tests/test_1_cat.txt" "tests/test_2_cat.txt" "tests/test_3_cat.txt")

for flag in "${flags[@]}"; do
    for file in "${files[@]}"; do
        run_test "$flag" "$file"
    done
done

echo "Успешных тестов: $success"
