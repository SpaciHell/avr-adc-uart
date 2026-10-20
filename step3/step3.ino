#include <avr/interrupt.h>

volatile char send_ = 1; //флаг нужно отправить или ещё нет

void uart_init() //функция для работы с uart
{
  UBRR0 = 103; //скорость передачи данных 9600 при 16 МГц
  UCSR0B = (1 << TXEN0); //включаем передачу
}

void setup() {
  uart_init();
  //используем таймер1
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12); //режим СТС(4 режим) и делитель 256, поэтому предыдущая строчка 0
  OCR1A = 62499; // срабатывает 1 раз в сек
  TIMSK1 = (1 << OCIE1A); //прерывние при достижении значения OCR1A
  sei();
}

ISR(TIMER1_COMPA_vect) { //сообщаем раз в секунду что пора отправить
  send_  = 1;
}

void uart_send_char(char c)//дождаться пока канал пуст и положить букву
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = c;
}

void uart_send_string(const char *s) //пройти по строке до '\0' и отправить каждую букву через uart_send_char
{
  while (*s != '\0') {
    uart_send_char(*s);
    s++;
  }
}

void loop() {
  if (send_  == 1) { //пришло ли время отпарвки
    send_  = 0; //сбрасываем до следующего раза
    uart_send_string("Nastuha molodec!!\r\n");
  }
}
