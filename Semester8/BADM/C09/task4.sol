// SPDX-License-Identifier: MIT
pragma solidity ^0.8.17;

contract EvenOdd {
    function isEven(uint number) public pure returns (bool) {
        if (number % 2 == 0) {
            return true;
        } else {
            return false;
        }
    }
}

