package jasonveins.omnet.decision;

public class ByteData{
    private short valueType;
    private byte[] value;

    public ByteData(short _valueType, byte[] _value){
        valueType = _valueType;
        value = _value;
    }

    public short valueType(){
        return valueType;
    }

    public byte[] value(){
        return value;
    }
}
