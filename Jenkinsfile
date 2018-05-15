pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('Checkout') {
      agent {
        docker {
          image 'webex-build'
        }

      }
      steps {
        git(credentialsId: 'mgruesen', url: 'git@github.com/mgruesen/libwebex', branch: 'master')
      }
    }
  }
}