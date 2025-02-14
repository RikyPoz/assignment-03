// LIB
#include <avr/sleep.h>
#include <stdlib.h>
#include <Arduino.h>
#include "window/Window.h"
#include "kernel/Scheduler.h"

// DEVICES
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "devices/Servo_motor_impl.h"
#include "devices/Display.h"

// TASKS
#include "tasks/ModalityTask.h"
#include "tasks/TaskReceivingMessage.h"
#include "tasks/WindowControlTask.h"
#include "tasks/TaskSending.h"

// COSTANT
#include "const.h"

// MAIN OBJ
Scheduler scheduler;
Window *window;

// SENSORS (stack)

ButtonImpl button(BTN_PIN);
ServoMotorImpl servo(SERVO_PIN);
Potentiometer pot(POT_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);
Display display(&lcd);

// TASKS

Task *receiveMessageTask;
Task *modalityTask;
Task *windowControlTask;
Task *sendingTask;


long t = 0;

void setup()
{
    MsgService.init();

    window = new Window(&button, &display, &pot, &servo);

    scheduler.init(SCHEDULER_BASE_PERIOD);

    // Creazione delle task

    modalityTask = new ModalityTask(window);
    modalityTask->init(MODALITY_PERIOD);

    windowControlTask = new WindowControlTask(window);
    windowControlTask->init(WINDOW_CONTROL_PERIOD);

    receiveMessageTask = new TaskReceivingMessage(window);
    receiveMessageTask->init(RECEIVING_MESSAGE_PERIOD);

    sendingTask = new TaskSending(window);
    sendingTask->init(SENDING_MESSAGE_PERIOD);

    scheduler.addTask(sendingTask);
    scheduler.addTask(modalityTask);
    scheduler.addTask(windowControlTask);
    scheduler.addTask(receiveMessageTask);
}

void loop()
{
    scheduler.schedule();
}
