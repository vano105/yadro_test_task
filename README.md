## Сборка
# Клонировать репозиторий
git clone https://github.com/vano105/yadro_test_task
cd yadro_test_task
# Скомпилировать проект
mkdir build && cd build && cmake .. && cmake --build .
# Запустить проект
./yadro ../test.txt
