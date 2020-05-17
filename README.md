Тут будет важная информация о PDP-11.
Инструкция по запуску трассировки:
1) Запустись с трассировкой 01_sum.pdp.txt
./out 01_sum.pdp.txt -t
2) Запустись с трассировкой 01_sum_mode1.pdp.txt
./out 01_sum_mode1.pdp.txt -t
3) Запустись с трассировкой 01_sum_neg.pdp.txt
./out 01_sum_neg.pdp.txt -t

Здесь out получен после команды 
gcc -o out pdp11_mm.c tests.c run.c

Ключ -t выступает как argv[2]. Он необязателен - если его не писать, то argv[2] = NULL, и программа запустится без выходных данных.
Написание argv[1] обязательно. В качестве него выступает один из текстовых файлов выше. Простой запуск ./out дает ошибку, так как нет входных данных, которые должны быть.

Описание моей трассировки:
В треугольных скобках - восьмеричный код каждого слова и статус выполяемой команды PDP-11 (то есть pc).
В круглых скобках - все слова, которые прочитались как add или mov. По факту, это все слова, кроме последнего halt. 
В квадратных скобках - трассировка работы sixbittodata. Там показано то, какой мод был использован. Подробности описания видно в коде pdp11_mm.c с 46 по 63 строки.
В моей трассировке также указаны значение и адрес каждого из аргументов dd и ss, полученных из 6 битных подпоследовательностей 16 битной последовательности (читаемого слова).

Upd.
Нашел у себя ошибку : У меня по непонятной причине все регистры кроме последнего были нулевые, в отличие от эталонного эмулятора, где они заполнялись числами. Оказалось, что дело
в том, что я просто забыл написать в регистр соответсвующие значения, а писал их лишь в оперативную память эмулятора. 
