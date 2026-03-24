.pragma library

//enum InputMethodHintの最大値が 0x800000 
var keybordType0 = 0x10000000;
var keybordType1 = 0x20000000;
var keybordType2 = 0x30000000;
var keybordType3 = 0x40000000;
var keybordType4 = 0x50000000;
var keybordType5 = 0x60000000;
var keybordType6 = 0x70000000;
var keybordType7 = 0x80000000;
var keybordTypeMask = 0xF0000000;

//Up/Downキーを表示/非表示切り替えする
function upDownKeyVisible(inputMethodHints){
    var keyType = inputMethodHints & 0xF0000000;
    switch(keyType){
    case keybordType0:
    case keybordType1:
    case keybordType2:
    case keybordType3:    
        return false;
    case keybordType4:
    case keybordType5:
    case keybordType5:
    case keybordType6:
    case keybordType7:
    default:
        return true;
    }
}

//複数文字キーを表示/非表示切り替えする
function multiCharKeyVisible(inputMethodHints){
    var keyType = inputMethodHints & 0xF0000000;
    switch(keyType){
    case keybordType0:
    case keybordType1:
    case keybordType4:
    case keybordType5:    
        return false;
    case keybordType2:
    case keybordType3:
    case keybordType5:
    case keybordType6:
    case keybordType7:
    default:
        return true;
    }
}

//Enterキーを表示/非表示切り替えする
function enterKeyVisible(inputMethodHints){
    var keyType = inputMethodHints & 0xF0000000;
    switch(keyType){
    case keybordType0:
    case keybordType2:
    case keybordType4:    
        return false;

    case keybordType1:
    case keybordType3:
    case keybordType5:
    case keybordType5:
    case keybordType6:
    case keybordType7:
    default:
        return true;
    }
}
