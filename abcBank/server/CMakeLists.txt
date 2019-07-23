cmake_minimum_required(VERSION 3.5)
project(my_muduo_net)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
add_compile_options("-std=c++11")

set(SERVER_SRCS 
  server.cc
  BankSession.cc
  TransactionManager.cc
  )

set(PUBLIC_SRCS
  ../public/PackStream.cc
  ../public/UnpackStream.cc
  ../public/StringUtil.cc
  ../public/Encrypt.cc
  ../public/Idea.cc
  ../public/MD5.cc
  )

set(CMD_SRCS
  ./CMD/UserLogin.cc
  ./CMD/OpenAccount.cc
  ./CMD/Deposit.cc
  ./CMD/Withdrawal.cc
  ./CMD/Transfer.cc
  ./CMD/BalanceInquiry.cc
  ./CMD/ChangePassword.cc
  ./CMD/QueryDayBill.cc
  )

set(DAL_SRCS
  ./DAL/BankService.cc
  ./DAL/MysqlDB.cc
  )

add_executable(server ${SERVER_SRCS} ${CMD_SRCS} ${PUBLIC_SRCS} ${DAL_SRCS})
target_link_libraries(server muduo_net muduo_base pthread)
target_link_libraries(server mysqlclient)

