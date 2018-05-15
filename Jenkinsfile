pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('Checkout') {
      steps {
        git(url: 'git@github.com/mgruesen/libwebex', branch: 'master', credentialsId: 'mgruesen')
      }
    }
  }
}