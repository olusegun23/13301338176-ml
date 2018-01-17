package com.example.global_lib;

import static javax.swing.text.html.HTML.Tag.BR;

/**
 * Created by haijunz on 18-1-15.
 */

public class User {

    static public String firstName;
    static public String lastName;

    public User(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
    }

}

/* public class User extends BaseObservable {
        private String firstName;
        private String lastName;
        @Bindable
        public String getFirstName() {
            return this.firstName;
        }
        @Bindable
        public String getLastName() {
            return this.lastName;
        }
        public void setFirstName(String firstName) {
            this.firstName = firstName;
            notifyPropertyChanged(BR.firstName);
        }
        public void setLastName(String lastName) {
            this.lastName = lastName;
            notifyPropertyChanged(BR.lastName);
        }
    }



*/

