#ifndef ETHERNETUTILS_H
#define ETHERNETUTILS_H

class EthernetUtils
{
  public:
    static void InitializeEthernet();
    static void SendUrlRequest(const char* action);

  protected:
  
  private:
    static void CheckEthernetHardwarePresence();
    static void CheckLinkStatus();
    static void PrintIpAddress();
    EthernetUtils();
};

#endif
