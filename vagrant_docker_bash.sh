set -e

if [ $(vagrant status --machine-readable | grep state,poweroff) ]; then
    vagrant up
fi

vagrant ssh -- -t "cd /vagrant/Plat4m_Core && ./docker_bash.sh $*"
