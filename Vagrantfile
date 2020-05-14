Vagrant.configure(2) do |config|

  # Import the Vagrant Box
  config.vm.box = "ubuntu/bionic64"
  config.vm.box_check_update = false
  config.vm.synced_folder ".", "/vagrant/"+File.basename(__dir__), type: "virtualbox"
  
  # VirtualBox Customization
  config.vm.provider "virtualbox" do |v|
    v.gui = false
      v.name = "plat4m_linux_dev_box"
      v.memory = 2048
      v.customize [
            "modifyvm", :id,
            "--cpus", 2,
            "--pae", "off",
            "--vram", 128,
            "--accelerate2dvideo", "off",
            "--accelerate3d", "off",
            "--usb", "on",
            "--usbehci", "on"
      ]
  end

  # Port Forwarding
  config.vm.network "forwarded_port", guest: 80, host: 8080
  
  # SSH Workaround for Non-interactive Login
  config.ssh.shell = "bash -c 'BASH_ENV=/etc/profile exec bash'"

  # Provision the VM
  config.vm.provision "shell", path: "vagrant-provision.sh"

end
