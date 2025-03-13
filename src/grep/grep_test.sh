#!/bin/bash

# Паттерн для тестирования
PATTERNS=("error" "hi")
FILE="test.txt"
FLAGS=("e" "i" "v" "c" "l" "h" "o")

# Счетчики для успешных и неуспешных тестов
SUCCESS_COUNT=0
FAIL_COUNT=0

run_test() {
    local pattern=$1
    local flag=$2

    # Запуск оригинального grep
    original_output=$(grep -$flag -e "$pattern" "$FILE")

    # Запуск вашей реализации s21_grep
    my_output=$(./s21_grep -$flag -e "$pattern" "$FILE")

    # Сравнение результатов
    if [ "$original_output" == "$my_output" ]; then
        echo "./s21_grep -$flag -e $pattern test.txt:  ACCEPTED"
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))  # Увеличиваем счетчик успешных тестов
        echo "______________________________"
        echo ""
    else
        echo "grep -$flag -e $pattern test.txt          FAILED"
        echo "Original grep output:"
        echo "$original_output"
        echo "My grep output:"
        echo "$my_output"
        echo "------------------------------"
        echo ""
        FAIL_COUNT=$((FAIL_COUNT + 1))  # Увеличиваем счетчик неуспешных тестов
    fi
}

# Тестирование для каждого паттерна и одного флага
for pattern in "${PATTERNS[@]}"; do
    for flag in "${FLAGS[@]}"; do
        run_test "$pattern" "$flag"
    done
done

# Тестирование для каждой комбинации из двух флагов
for (( i=0; i<${#FLAGS[@]}; i++ )); do
    for (( j=i+1; j<${#FLAGS[@]}; j++ )); do
        combined_flags="${FLAGS[i]}${FLAGS[j]}"
        for pattern in "${PATTERNS[@]}"; do
            run_test "$pattern" "$combined_flags"
        done
    done
done

# Тестирование для каждой комбинации из трех флагов
for (( i=0; i<${#FLAGS[@]}; i++ )); do
    for (( j=i+1; j<${#FLAGS[@]}; j++ )); do
        for (( k=j+1; k<${#FLAGS[@]}; k++ )); do
            combined_flags="${FLAGS[i]}${FLAGS[j]}${FLAGS[k]}"
            for pattern in "${PATTERNS[@]}"; do
                run_test "$pattern" "$combined_flags"
            done
        done
    done
done

# Тестирование для каждой комбинации из четырех флагов
for (( i=0; i<${#FLAGS[@]}; i++ )); do
    for (( j=i+1; j<${#FLAGS[@]}; j++ )); do
        for (( k=j+1; k<${#FLAGS[@]}; k++ )); do
            for (( l=k+1; l<${#FLAGS[@]}; l++ )); do
                combined_flags="${FLAGS[i]}${FLAGS[j]}${FLAGS[k]}${FLAGS[l]}"
                for pattern in "${PATTERNS[@]}"; do
                    run_test "$pattern" "$combined_flags"
                done
            done
        done
    done
done

# Тестирование для каждой комбинации из пяти флагов (все флаги)
combined_flags="${FLAGS[0]}${FLAGS[1]}${FLAGS[2]}${FLAGS[3]}${FLAGS[4]}"
for pattern in "${PATTERNS[@]}"; do
    run_test "$pattern" "$combined_flags"
done

# Вывод результатов тестирования
echo "Tests completed."
echo "Successful tests: $SUCCESS_COUNT"
echo "Failed tests: $FAIL_COUNT"
