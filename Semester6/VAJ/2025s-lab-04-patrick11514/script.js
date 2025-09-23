// This is script which we can utilize to seed our database.

import { PrismaClient } from '@prisma/client'

const prisma = new PrismaClient()

const CreateUser = async () => {
  //  If you want to create more "sample" users, change the name and email
  const user = await prisma.user.create({
    data: {
      name: 'Alice',
      email: 'alice@prisma.io',
    },
  })
  console.log(user)
}


// Run the script

try{
    await CreateUser()
    await prisma.$disconnect()
} catch (e) {
    console.error(e)
    await prisma.$disconnect()
    process.exit(1)
}