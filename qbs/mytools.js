
function pobj(obj)
{
    var s = "-----------\r";
    for(i in obj)
    {
        s += i + "=" + obj[i] + "\r";
    }
    s += "-----------\r";
//    throw s;

    console.info(s)

}

