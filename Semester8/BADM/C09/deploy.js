#!/usr/bin/env node
const { default: Web3 } = require('web3');
const fs = require('fs');
const solc = require('solc');

class ContractDeploy {
  constructor(providerUrl, privateKey) {
    this.web3 = new Web3(providerUrl);

    this.account = this.web3.eth.accounts.privateKeyToAccount(privateKey);
    this.web3.eth.accounts.wallet.add(this.account);
    this.web3.eth.defaultAccount = this.account.address;
  }

  async compileContract(filePath) {
    const source = fs.readFileSync(filePath, 'utf8');

    const input = {
      language: 'Solidity',
      sources: { [filePath]: { content: source } },
      settings: {
        outputSelection: { '*': { '*': ['abi', 'evm.bytecode'] } }
      }
    };

    const output = JSON.parse(solc.compile(JSON.stringify(input)));
    if (output.errors) {
      const fatals = output.errors.filter(e => e.severity === 'error');
      if (fatals.length) throw new Error(fatals.map(e => e.formattedMessage).join('\n'));
    }

    fs.writeFileSync('compiled_code.json', JSON.stringify(output, null, 2));
    return output;
  }

  async deployContract(contractFile, contractName, gasLimit, gasPriceGwei) {
    const compiled = JSON.parse(fs.readFileSync('compiled_code.json'));
    const { abi, evm: { bytecode } } = compiled.contracts[contractFile][contractName];

    const contract = new this.web3.eth.Contract(abi);
    const deployTx = contract.deploy({ data: '0x' + bytecode.object });

    const receipt = await deployTx.send({
      from: this.account.address,
      gas: gasLimit,
      gasPrice: this.web3.utils.toWei(gasPriceGwei.toString(), 'gwei')
    });

    this.contractInstance = new this.web3.eth.Contract(abi, receipt.options.address);
    return receipt.options.address;
  }

  async callFunction(functionName, args = [], gasLimit, gasPriceGwei) {
    if (!this.contractInstance) throw new Error('Nejprve deploy nebo load_contract');
    const tx = this.contractInstance.methods[functionName](...args);
    const receipt = await tx.send({
      from: this.account.address,
      gas: gasLimit,
      gasPrice: this.web3.utils.toWei(gasPriceGwei.toString(), 'gwei')
    });
    return receipt;
  }

  async getValue(functionName) {
    if (!this.contractInstance) throw new Error('Nejprve deploy nebo load_contract');
    return this.contractInstance.methods[functionName]().call();
  }

  loadContract(address, abi) {
    this.contractInstance = new this.web3.eth.Contract(abi, address);
  }
}

(async () => {
  try {
    const provider = 'https://eth-sepolia.g.alchemy.com/v2/<API_KEY>';
    const pk       = '0x<PRIVATE_KEY>';
    const deployer = new ContractDeploy(provider, pk);

    await deployer.compileContract('py_sol.sol');

    const addr = await deployer.deployContract('py_sol.sol', 'SimpleStorage', 2_000_000, 50);
    console.log('Deployed at:', addr);

    const txr = await deployer.callFunction('set', [123], 200_000, 50);
    console.log('set() tx hash:', txr.transactionHash);

    const val = await deployer.getValue('get');
    console.log('Value is:', val);
  } catch (e) {
    console.error(e);
  }
})();
