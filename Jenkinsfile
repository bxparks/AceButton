pipeline {
    agent { label 'master' }
    environment {
        AUNITER_ARDUINO_BINARY = '/var/lib/jenkins/arduino-1.8.5/arduino'
        AUNITER_CONFIG = 'AUniterConfig'
    }
    stages {
        stage('Build') { 
            steps {
                /var/lib/jenkins/arduino-1.8.5/portable/sketchbook/libraries/AUnit/tools/auniter.sh --boards nano --verify tests/AceButtonTest
            }
        }
    }
}
