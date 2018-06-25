pipeline {
    agent { label 'master' }
    environment {
        AUNITER_ARDUINO_BINARY = '/var/lib/jenkins/arduino-1.8.5/arduino'
        AUNITER_CONFIG = 'AceButton/AUniterConfig'
    }
    stages {
        stage('Setup') {
            steps {
                dir('AUnit') {
                    git url: 'https://github.com/bxparks/AUnit'
                }
            }
        }

        stage('Build') { 
            steps {
                sh 'AUnit/tools/auniter.sh --boards nano --verify AceButton/tests/AceButtonTest'
            }
        }
    }
}
