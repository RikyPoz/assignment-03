#include "TaskReceivingMessage.h"
#include "kernel/MsgService.h"

TaskReceivingMessage::TaskReceivingMessage(Window *window)
{
    this->window = window;
}

void TaskReceivingMessage::tick()
{
    if (MsgService.isMsgAvailable())
    {
        Serial.println("ricevo");
        String msg = MsgService.receiveMsg()->getContent();
        // se facciamo 2 msg separati
        if (msg.startsWith("temperature_"))
        {
            String temp = msg.substring(11);
            window->updateTemp(temp);
        }
        else if (msg.startsWith("level_"))
        {
            int level = msg.substring(6).toInt();
            window->updateWindowLevel(level);
        }
        else
        {
            Serial.println("Error: Unknown message");
        }

        /* se facciamo un unico msg "temperature_X,stato_Y"
        int separatorIndex = msg.indexOf(',');
        if (separatorIndex != -1) {
            String tempStr = msg.substring(11, separatorIndex);
            String stateStr = msg.substring(separatorIndex + 1);

            float temp = tempStr.toFloat();

            window->updateTemp(temp);
            window->updateState(stateStr);*/
    }
}
