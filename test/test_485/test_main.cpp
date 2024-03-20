#include <Arduino.h>
#include <unity.h>

String STR_TO_TEST;

int ccc;
void setUp(void) {
    // set stuff up here
    STR_TO_TEST = "Hello, world!";
}

void tearDown(void) {
    // clean stuff up here
    STR_TO_TEST = "";
}

void test_string_concat(void) {
    String hello = "Hello, ";
    String world = "world!";
    TEST_ASSERT_EQUAL_STRING(STR_TO_TEST.c_str(), (hello + world).c_str());
}

void test_string_substring(void) {
    TEST_ASSERT_EQUAL_STRING("Hello", STR_TO_TEST.substring(0, 5).c_str());
}

void test_string_index_of(void) {
    TEST_ASSERT_EQUAL(7, STR_TO_TEST.indexOf('w'));
}

void test_string_equal_ignore_case(void) {
    TEST_ASSERT_TRUE(STR_TO_TEST.equalsIgnoreCase("HELLO, WORLD!"));
}

void test_string_to_upper_case(void) {
    STR_TO_TEST.toUpperCase();
    TEST_ASSERT_EQUAL_STRING("HELLO, WORLD!", STR_TO_TEST.c_str());
}

void test_string_replace(void) {
    STR_TO_TEST.replace('!', '?');
    TEST_ASSERT_EQUAL_STRING("Hello, world?", STR_TO_TEST.c_str());
}

#define OP_LED 33
#define TX2_PIN 12   // Serial2 RX 핀 (GPIO12)
#define RX2_PIN 14   // Serial2 TX 핀 (GPIO14)
void test_char_equal(){
  TEST_ASSERT_EACH_EQUAL_HEX8('A',ccc,1);

}
void setup()
{
  delay(2000); // service delay

  pinMode(OP_LED, OUTPUT);
  Serial.begin(BAUDRATEDEF);
  pinMode(OP_LED, OUTPUT);
  digitalWrite(OP_LED, 0); // receive mode
  Serial.printf("\r\nUsing 485 for Battery communication");
  Serial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN); // for 485
  digitalWrite(OP_LED, 0); // Receive mode
  UNITY_BEGIN();
  while(1){
    if(Serial2.available()){
      ccc=Serial2.read();
      RUN_TEST(test_char_equal);
    }
    //RUN_TEST(test_char_equal);
    delay(10);
  }
  // int count = 100;
  // while (count--)
  // {
  //   RUN_TEST(test_string_concat);
  //   RUN_TEST(test_string_substring);
  //   RUN_TEST(test_string_index_of);
  //   RUN_TEST(test_string_equal_ignore_case);
  //   RUN_TEST(test_string_to_upper_case);
  //   RUN_TEST(test_string_replace);
  //   digitalWrite(OP_LED, 1); // Write mode
  //   delay(1);
  //   Serial2.print("\n\rHello..");
  //   Serial2.flush();
  //   digitalWrite(OP_LED, 0); // Receive mode
  //   delay(10);
  //   delay(1000);
  // }

  UNITY_END(); // stop unit testing
}

void loop()
{
    delay(2000);
}
