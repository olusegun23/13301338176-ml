package com.example.global_lib;

public class globalData {

    //public globalData  (String lib1_result) {this.lib1_result= lib1_result;}

    static public String lib1_result="unknow";
    static public String salutation;
    public globalData(String salutation) {this.salutation = salutation;}
    public static globalData get(String salutation) {return new globalData(salutation);}
    public String getMessage() {return String.format("=== %s", salutation);}

}
