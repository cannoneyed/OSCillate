class OSCManager
{
public:
    
    OSCManager () {}
    
    OSCManager (String ipValue, int portValue, String addressValue) : ip (ipValue),
    port (portValue),
    address (addressValue)
    { }
    
    void setIP (String toSet) { ip = toSet; }
    void setPort (int toSet) { port = toSet; }
    void setAddress (String toSet)
    {
        oscFormat(toSet);
        address = toSet;
    }
    
    
    bool checkIP (String toCheck)
    {
        String dots = toCheck.retainCharacters(".");
        if (dots.length() != 3)
            return false;
        
        Array <int> bytes;
        for (int i = 0; i < 3; i++) {
            bytes.add(toCheck.upToFirstOccurrenceOf(".", false, false).getIntValue());
            toCheck = toCheck.fromFirstOccurrenceOf(".", false, false);
        }
        bytes.add(toCheck.getIntValue());
        
        for (int i = 0; i < 4; i++) {
            if (bytes [i] > 255 || bytes [i] < 0)
                return false;
        }
        return true;
    }
    
    bool checkPort (int toCheck)
    {
        if (toCheck < 0 || toCheck > 65535)
            return false;
        else
            return true;
    }
    
    void oscFormat (String &toFormat)
    {
        toFormat = toFormat.removeCharacters(" #*,?[]{}");
        
        if (toFormat.substring(0, 1) != "/")
            toFormat = "/" + toFormat;
        if (toFormat.substring(toFormat.length()-1, toFormat.length()) == "/")
            toFormat = toFormat.substring(0, toFormat.length()-1);
    }
    
    
    String getIP () { return ip; }
    int getPort () { return port; }
    String getAddress () { return address; }
    
private:
    String ip;
    int port;
    String address;
    
};