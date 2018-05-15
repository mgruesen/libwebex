pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('Checkout') {
      steps {
        git(credentialsId: 'mgruesen', url: 'git@github.com/mgruesen/libwebex', branch: 'master')
      }
    }
  }
}