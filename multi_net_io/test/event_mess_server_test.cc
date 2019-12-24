#include "event_mess_handle.h"
#include "test_config.h"

using namespace std;


 void server_recive_cb(EventMessageHandle * mess_handle,EventMessage * mess,void * arg)
 {
     EventMessage reply;
     cout << "Recive from Group "<<mess->group_name<<"["<<mess->mess_type<<"] : " << mess->message << endl;
     reply.prepare_send(mess->group_name,mess->mess_type,mess->send_host_name,mess->message,mess->message_size);
     mess_handle->sendMessage(&reply);
 }

int main(void)
{
    EventMessageHandle handler;

    string host_config_path = std::string(TEST_SOURCE_DIR) + "host_config2.json";
    string mess_config_path = std::string(TEST_SOURCE_DIR) + "mess_config.json";
    cout << "host config file path = " << host_config_path << endl << "message config file path = " << mess_config_path << endl;
    handler.init_handle(host_config_path.c_str(),mess_config_path.c_str());

    handler.register_recive_handler("admin","reply",server_recive_cb,NULL);

    cout << "Message Server Test" << endl;
    //cout << "---------- Message Callback Recive Test ------------" << endl;

    int count = 0;
    int return_code =0;
    while(1){
        cout << "---------- Message Constantly Recive Test ------------" << endl;
        EventMessage while_mess;

        while_mess.prepare_recive("admin","heart");
        return_code = handler.readMessage(&while_mess);
        cout << "readMessage return_code : " << return_code << endl;
        if(return_code <= 0)
        {
             cout << "read error" << std::endl;
             cout << MessageError::getEventErrorStr(while_mess.error_no) << endl;
        }
        else
        {
            cout <<"READ RESULT : " <<while_mess.message << std::endl;
            cout <<"READ MESS TYPE : " <<while_mess.mess_type << std::endl;
            count++;
        }
        sleep(5);
    }
}