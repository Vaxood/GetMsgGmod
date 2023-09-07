# GetMsgGmod
get the msg message in the console in glua
usage : 
```
require("sora_msg")
hook.Add("OnConMsgCalled","vaxod",function(msg)

    print("called from lua",msg)  -- msg its the message in the console
    return false -- return false if u want to not print it into the console
end)

```


