/**
 * @file i2c_slave.hpp
 * @brief Inter-Integrated Circuit(I2C) SLAVE API prototypes
 *
 *
 * @date December 7th, 2023
 * @copyright Copyright (c) 2023 Limitless Aeronautics
 *
 * @author Lukas R. Jackson (LukasJacksonEG@gmail.com)
 *
 * @license MIT License
 *          Copyright (c) 2023 limitless Aeronautics
 *          Permission is hereby granted, free of charge, to any person obtaining a copy
 *          of this software and associated documentation files (the "Software"), to deal
 *          in the Software without restriction, including without limitation the rights
 *          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *          copies of the Software, and to permit persons to whom the Software is
 *          furnished to do so, subject to the following conditions:
 *          The above copyright notice and this permission notice shall be included in all
 *          copies or substantial portions of the Software.
 *          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *          IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *          FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *          AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *          LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *          OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *          SOFTWARE.
 */
#ifndef i2c_slave_h
#define i2c_slave_h

/**
 * @brief I2C Communication Overview
 *
 * @date December 7th, 2023
 * @author Lukas R. Jackson (LukasJacksonEG@gmail.com)
 *
 * @section I2C Communication Overview
 *
 * I2C (Inter-Integrated Circuit) is a widely used serial communication protocol that allows multiple devices
 * to communicate with each other using only two wires: SDA (Serial Data) and SCL (Serial Clock).
 *
 *
 * @par Visual Representation:
 *
 * @code{.unparsed}
 * +-------------------------+
 * |      Master Device      |
 * |         +---------+      |
 * |         |         |      |
 * | SDA o---|         |      |
 * |         |         |      |
 * | SCL o---|         |      |
 * |         |         |      |
 * |         +---------+      |
 * |                         |
 * |      Slave Device 1     |
 * |         +---------+      |
 * |         |         |      |
 * | SDA o---|         |      |
 * |         |         |      |
 * | SCL o---|         |      |
 * |         |         |      |
 * |         +---------+      |
 * |                         |
 * |      Slave Device 2     |
 * |         +---------+      |
 * |         |         |      |
 * | SDA o---|         |      |
 * |         |         |      |
 * | SCL o---|         |      |
 * |         |         |      |
 * |         +---------+      |
 * +-------------------------+
 * @endcode
 *
 * @par Explanation:
 *
 * - The Master device initiates communication and generates the clock signal (SCL).
 * - The Master sends the Start condition to begin communication.
 * - Each Slave device on the bus has a unique address.
 * - The Master sends the address of the intended Slave along with the Read/Write bit.
 * - The addressed Slave acknowledges its presence.
 * - Data transfer occurs in 8-bit chunks.
 * - Acknowledgments or NACKs are sent by the receiver.
 * - The Master generates the Stop condition to end communication.
 *
 * I2C is known for its simplicity and is widely used in various applications, such as connecting sensors,
 * EEPROMs, and other integrated circuits in embedded systems.
 */

/**
 * @brief I2C Connection between Two ESP32 Boards
 *
 * @date December 7th, 2023
 * @author Lukas R. Jackson (LukasJacksonEG@gmail.com)
 *
 * @section i2c_connection_sec I2C Connection Diagram
 *
 * @code{.unparsed}
 * +-----------------------------------------------+
 * |                ESP32 Board 1                  |
 * |                                               |
 * |                 +------------+                |
 * |                 |            |                |
 * |           +-----| SDA (21)   |                |
 * |           |     |            |                |
 * |           |     +------------+                |
 * |           |                                   |
 * |           |                                   |
 * |           |     +------------+                |
 * |           |     |            |                |
 * |           +-----| SCL (22)   |                |
 * |                 |            |                |
 * |                 +------------+                |
 * |                                               |
 * |                 +------------+                |
 * |                 |            |                |
 * |           +-----| GND        |                |
 * |           |     |            |                |
 * |           |     +------------+                |
 * |           |                                   |
 * +-----------------------------------------------+
 * +-----------------------------------------------+
 * |                 ESP32 Board 2                 |
 * |                                               |
 * |                 +------------+                |
 * |                 |            |                |
 * |           +-----| SDA (21)   |                |
 * |           |     |            |                |
 * |           |     +------------+                |
 * |           |                                   |
 * |           |                                   |
 * |           |     +------------+                |
 * |           |     |            |                |
 * |           +-----| SCL (22)   |                |
 * |                 |            |                |
 * |                 +------------+                |
 * |                                               |
 * |                 +------------+                |
 * |                 |            |                |
 * |           +-----| GND        |                |
 * |           |     |            |                |
 * |           |     +------------+                |
 * +-----------------------------------------------+
 * @endcode
 *
 * @par Connection Details:
 *
 * - Connect the SDA pin (21) of Board 1 to the SDA pin (21) of Board 2.
 * - Connect the SCL pin (22) of Board 1 to the SCL pin (22) of Board 2.
 * - Connect the GND (Ground) of Board 1 to the GND (Ground) of Board 2.
 *
 * This establishes a basic I2C connection between the two ESP32 boards, allowing them to communicate
 * using the I2C protocol.
 */

#pragma once

#ifndef __cpp_exceptions
#error I2C class can only be used when __cpp_exceptions is enabled. Enable CONFIG_COMPILER_CXX_EXCEPTIONS in Kconfig
#endif

#include <exception>
#include <memory>
#include <chrono>
#include <vector>
#include <list>
#include <future>

#include "sdkconfig.h"
#include "esp_exception.hpp"
#include "system_cxx.hpp"
#include "gpio_cxx.hpp"
#include "driver/i2c.h"
#include "esp_log.h"
#include "../Logging/logger.hpp"

#include <stdio.h>
#include <string.h>

/* Predefined I2C Slave Macros */
static const char *TAG = "i2c-slave";

#define LED_PIN 2

// #define I2C_SLAVE_SCL_IO 22      /*!< gpio number for I2C master clock */
// #define I2C_SLAVE_SDA_IO 21      /*!< gpio number for I2C master data  */
#define I2C_SLAVE_FREQ_HZ 100000 /*!< I2C master clock frequency */
#define I2C_SLAVE_TX_BUF_LEN 255 /*!< I2C master doesn't need buffer */
#define I2C_SLAVE_RX_BUF_LEN 255 /*!< I2C master doesn't need buffer */
#define ESP_SLAVE_ADDR 0x0A

#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                /*!< I2C ack value */
#define NACK_VAL 0x1               /*!< I2C nack value */

namespace idf
{

    /**
     * @brief Check if the provided numerical value is a valid I2C address.
     *
     * @param addr raw number to be checked.
     * @return ESP_OK if \c addr is a valid I2C address, otherwise ESP_ERR_INVALID_ARG.
     */
    esp_err_t check_i2c_addr(uint32_t addr) noexcept;

    struct I2CException : public ESPException
    {
        I2CException(esp_err_t error);
    };

    struct I2CTransferException : public I2CException
    {
        I2CTransferException(esp_err_t error);
    };

    /**
     * @brief Represents a valid SDA signal pin number.
     */
    class SDA_type;
    using SDA_GPIO = GPIONumBase<class SDA_type>;

    /**
     * @brief Represents a valid SCL signal pin number.
     */
    class SCL_type;
    using SCL_GPIO = GPIONumBase<class SCL_type>;

    /**
     * @brief Valid representation of I2C number.
     *
     * A chip can have multiple I2C interfaces, each identified by a bus number, subsequently called I2C number.
     * Instances of this class are guaranteed to always contain a valid I2C number.
     */
    class I2CNumber : public StrongValueComparable<uint32_t>
    {
        /**
         * Construct a valid representation of the I2C number.
         *
         * This constructor is private because the it can only be accessed but the static creation methods below.
         * This guarantees that an instance of I2CNumber always carries a valid number.
         */
        constexpr explicit I2CNumber(uint32_t number) : StrongValueComparable<uint32_t>(number) {}

    public:
        /**
         * @brief create an I2C number representing the first I2C bus of the chip.
         */
        constexpr static I2CNumber I2C0()
        {
            return I2CNumber(0);
        }

#if CONFIG_SOC_I2C_NUM == 2
        /**
         * @brief create an I2C number representing the second I2C bus of the chip.
         */
        constexpr static I2CNumber I2C1()
        {
            return I2CNumber(1);
        }
#endif
    };

    /**
     * @brief Valid representation of I2C address.
     *
     * Instances of this class are guaranteed to always contain a valid I2C address.
     */
    class I2CAddress : public StrongValueComparable<uint8_t>
    {
    public:
        /**
         *
         */
        explicit I2CAddress(uint8_t addr);
    };

    /**
     * @brief Low-level I2C transaction descriptor
     *
     * This class records and decribes a low-level transaction. Users use the methods (except \c execute_transfer)
     * to record the transaction. Afterwards, the transaction will be executed by calling \c execute_transfer,
     * which blocks until the transaction is finished.
     *
     * @note This is a low-level class, which only exists due to the underlying I2C driver. All data referenced in
     *      read and write calls must not be changed and must stay allocated until at least \c execute_transfer
     *      has finished.
     */
    class I2CCommandLink
    {
    public:
        /**
         * @brief Allocate and create the transaction descriptor.
         */
        I2CCommandLink();

        /**
         * @brief Delete the transaction descriptor, de-allocate all resources.
         */
        ~I2CCommandLink();

        I2CCommandLink(const I2CCommandLink &) = delete;
        I2CCommandLink operator=(const I2CCommandLink &) = delete;

        /**
         * @brief Record a start signal on the I2C bus.
         */
        void start();

        /**
         * @brief Record a write of the vector \c bytes on the I2C bus.
         *
         * @param[in] bytes The data to be written. Must stay allocated until execute_transfer has finished or
         *          destructor of this class has been called.
         * @param[in] expect_ack If acknowledgement shall be requested after each written byte, pass true,
         *          otherwise false.
         */
        void write(const std::vector<uint8_t> &bytes, bool expect_ack = true);

        /**
         * @brief Record a one-byte-write on the I2C bus.
         *
         * @param[in] byte The data to be written. No restrictions apply.
         * @param[in] expect_ack If acknowledgement shall be requested after writing the byte, pass true,
         *          otherwise false.
         */
        void write_byte(uint8_t byte, bool expect_ack = true);

        /**
         * @brief Record a read of the size of vector \c bytes on the I2C bus.
         *
         * @param[in] bytes Vector with the size of the data to be read (in bytes). Must stay allocated until
         *          execute_transfer has finished or destructor of this class has been called.
         * @param[in] expect_ack If acknowledgement shall be requested after each written byte, pass true,
         *          otherwise false.
         */
        void read(std::vector<uint8_t> &bytes);

        /**
         * @brief Record a stop command on the I2C bus.
         */
        void stop();

        /**
         * @brief Execute the transaction and wait until it has finished.
         *
         * This method will issue the transaction with the operations in the order in which they have been recorded
         * before.
         *
         * @param i2c_num I2C bus number on the chip.
         * @param driver_timeout Timeout for this transaction.
         */
        void execute_transfer(I2CNumber i2c_num, std::chrono::milliseconds driver_timeout);

    private:
        /**
         * @brief Internal driver data.
         */
        void *handle;
    };

    /**
     * Superclass for all transfer objects which are accepted by \c I2CMaster::transfer().
     */
    template <typename TReturn>
    class I2CTransfer
    {
    public:
        /**
         * Helper typedef to facilitate type resolution during calls to I2CMaster::transfer().
         */
        typedef TReturn TransferReturnT;

        /**
         * @param driver_timeout The timeout used for calls like i2c_master_cmd_begin() to the underlying driver.
         */
        I2CTransfer(std::chrono::milliseconds driver_timeout_arg = std::chrono::milliseconds(1000));

        virtual ~I2CTransfer() {}

        /**
         * Do all general parts of the I2C transfer:
         *  - initialize the command link
         *  - issuing a start to the command link queue
         *  - calling \c queue_cmd() in the subclass to issue specific commands to the command link queue
         *  - issuing a stop to the command link queue
         *  - executing the assembled commands on the I2C bus
         *  - calling \c process_result() to process the results of the commands or calling process_exception() if
         *    there was an exception
         *  - deleting the command link
         * This method is normally called by I2CMaster, but can also be used stand-alone if the bus corresponding to
         * \c i2c_num has be initialized.
         *
         * @throws I2CException for any particular I2C error
         */
        TReturn do_transfer(I2CNumber i2c_num, I2CAddress i2c_addr);

    protected:
        /**
         * Implementation of the I2C command is implemented by subclasses.
         * The I2C command handle is initialized already at this stage.
         * The first action is issuing the I2C address and the read/write bit, depending on what the subclass implements.
         * On error, this  method has to throw an instance of I2CException.
         *
         * @param handle the initialized command handle of the I2C driver.
         * @param i2c_addr The slave's I2C address.
         *
         * @throw I2CException
         */
        virtual void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) = 0;

        /**
         * Implementation of whatever neccessary action after successfully sending the I2C command.
         * On error, this  method has to throw an instance of I2CException.
         *
         * @throw I2CException
         */
        virtual TReturn process_result() = 0;

        /**
         * For some calls to the underlying driver (e.g. \c i2c_master_cmd_begin() ), this general timeout will be passed.
         */
        std::chrono::milliseconds driver_timeout;
    };

    /**
     * @brief Super class for any I2C master or slave
     */
    class I2CBus
    {
    public:
        /*
         * @brief Initialize I2C master bus.
         *
         * Initialize and install the bus driver in master mode.
         *
         * @param i2c_number The I2C port number.
         */
        explicit I2CBus(I2CNumber i2c_number);

        /**
         * @brief uninstall the bus driver.
         */
        virtual ~I2CBus();

        /**
         * The I2C port number.
         */
        const I2CNumber i2c_num;
    };

    /**
     * @brief Simple I2C Master object
     *
     * This class provides to ways to issue I2C read and write requests. The simplest way is to use \c sync_write() and
     * sync_read() to write and read, respectively. As the name suggests, they block during the whole transfer.
     * For all asynchrounous transfers as well as combined write-read transfers, use \c transfer().
     */
    class I2CMaster : public I2CBus
    {
    public:
        /**
         * Initialize and install the driver of an I2C master peripheral.
         *
         * Initialize and install the bus driver in master mode. Pullups will be enabled for both pins. If you want a
         * different configuration, use configure() and i2c_set_pin() of the underlying driver to disable one or both
         * pullups.
         *
         * @param i2c_number The number of the I2C device.
         * @param scl_gpio GPIO number of the SCL line.
         * @param sda_gpio GPIO number of the SDA line.
         * @param clock_speed The master clock speed.
         * @param scl_pullup Enable SCL pullup.
         * @param sda_pullup Enable SDA pullup.
         *
         * @throws I2CException with the corrsponding esp_err_t return value if something goes wrong
         */
        explicit I2CMaster(I2CNumber i2c_number,
                           SCL_GPIO scl_gpio,
                           SDA_GPIO sda_gpio,
                           Frequency clock_speed,
                           bool scl_pullup = true,
                           bool sda_pullup = true);

        /**
         * Delete the driver.
         */
        virtual ~I2CMaster();

        /**
         * Issue an asynchronous I2C transfer which is executed in the background.
         *
         * This method uses a C++ \c std::future as mechanism to wait for the asynchronous return value.
         * The return value can be accessed with \c future::get(). \c future::get() also synchronizes with the thread
         * doing the work in the background, i.e. it waits until the return value has been issued.
         *
         * The actual implementation is delegated to the TransferT object. It will be given the I2C number to work
         * with.
         *
         * Requirements for TransferT: It should implement or imitate the interface of I2CTransfer.
         *
         * @param xfer The transfer to execute. What the transfer does, depends on it's implementation in
         *      \c TransferT::do_transfer(). It also determines the future template of this function, indicated by
         *      \c TransferT::TransferReturnT.
         *
         * @param i2c_addr The address of the I2C slave device targeted by the transfer.
         *
         * @return A future with \c TransferT::TransferReturnT. It depends on which template type is used for xfer.
         *         In case of a simple write (I2CWrite), it's future<void>.
         *         In case of a read (I2CRead), it's future<vector<uint8_t> > corresponding to the length of the read
         *         operation.
         *         If TransferT is a combined transfer with repeated reads (I2CComposed), then the return type is
         *         future<vector<vector<uint8_t> > >, a vector of results corresponding to the queued read operations.
         *
         * @throws I2CException with the corrsponding esp_err_t return value if something goes wrong
         * @throws std::exception for failures in libstdc++
         */
        template <typename TransferT>
        std::future<typename TransferT::TransferReturnT> transfer(I2CAddress i2c_addr, std::shared_ptr<TransferT> xfer);

        /**
         * Do a synchronous write.
         *
         * All data in data will be written to the I2C device with i2c_addr at once.
         * This method will block until the I2C write is complete.
         *
         * @param i2c_addr The address of the I2C device to which the data shall be sent.
         * @param data The data to send (size to be sent is determined by data.size()).
         *
         * @throws I2CException with the corrsponding esp_err_t return value if something goes wrong
         * @throws std::exception for failures in libstdc++
         */
        void sync_write(I2CAddress i2c_addr, const std::vector<uint8_t> &data);



        /**
         * Do a synchronous read.
         * This method will block until the I2C read is complete.
         *
         * n_bytes bytes of data will be read from the I2C device with i2c_addr.
         * While reading the last byte, the master finishes the reading by sending a NACK, before issuing a stop.
         *
         * @param i2c_addr The address of the I2C device from which to read.
         * @param n_bytes The number of bytes to read.
         *
         * @return the read bytes
         *
         * @throws I2CException with the corrsponding esp_err_t return value if something goes wrong
         * @throws std::exception for failures in libstdc++
         */
        std::vector<uint8_t> sync_read(I2CAddress i2c_addr, size_t n_bytes);

        /**
         * Do a simple synchronous write-read transfer.
         *
         * First, \c write_data will be written to the bus, then a number of \c read_n_bytes will be read from the bus
         * with a repeated start condition. The slave device is determined by \c i2c_addr.
         * While reading the last byte, the master finishes the reading by sending a NACK, before issuing a stop.
         * This method will block until the I2C transfer is complete.
         *
         * @param i2c_addr The address of the I2C device from which to read.
         * @param write_data The data to write to the bus before reading.
         * @param read_n_bytes The number of bytes to read.
         *
         * @return the read bytes
         *
         * @throws I2CException with the corrsponding esp_err_t return value if something goes wrong
         * @throws std::exception for failures in libstdc++
         */
        std::vector<uint8_t> sync_transfer(I2CAddress i2c_addr,
                                           const std::vector<uint8_t> &write_data,
                                           size_t read_n_bytes);
    };

#if CONFIG_SOC_I2C_SUPPORT_SLAVE
    /**
     * @brief Responsible for initialization and de-initialization of an I2C slave peripheral.
     */
    class I2CSlave : public I2CBus
    {
    public:
        /**
         * Initialize and install the driver of an I2C slave peripheral.
         *
         * Initialize and install the bus driver in slave mode. Pullups will be enabled for both pins. If you want a
         * different configuration, use configure() and i2c_set_pin() of the underlying driver to disable one or both
         * pullups.
         *
         * @param i2c_number The number of the I2C device.
         * @param scl_gpio GPIO number of the SCL line.
         * @param sda_gpio GPIO number of the SDA line.
         * @param slave_addr The address of the slave device on the I2C bus.
         * @param rx_buf_len Receive buffer length.
         * @param tx_buf_len Transmit buffer length.
         * @param scl_pullup Enable SCL pullup.
         * @param sda_pullup Enable SDA pullup.
         *
         * @throws
         */
        I2CSlave(I2CNumber i2c_number,
                 SCL_GPIO scl_gpio,
                 SDA_GPIO sda_gpio,
                 I2CAddress slave_addr,
                 size_t rx_buf_len,
                 size_t tx_buf_len,
                 bool scl_pullup = true,
                 bool sda_pullup = true);

        /**
         * Delete the driver.
         */
        virtual ~I2CSlave();

        /**
         * Schedule a raw data write once master is ready.
         *
         * The data is saved in a buffer, waiting for the master to pick it up.
         */
        virtual int write_raw(const uint8_t *data, size_t data_len, std::chrono::milliseconds timeout);

        /**
         * Read raw data from the bus.
         *
         * The data is read directly from the buffer. Hence, it has to be written already by master.
         */
        virtual int read_raw(uint8_t *buffer, size_t buffer_len, std::chrono::milliseconds timeout);
    };
#endif // CONFIG_SOC_I2C_SUPPORT_SLAVE

    /**
     * Implementation for simple I2C writes, which can be executed by \c I2CMaster::transfer().
     * It stores the bytes to be written as a vector.
     */
    class I2CWrite : public I2CTransfer<void>
    {
    public:
        /**
         * @param bytes The bytes which should be written.
         * @param driver_timeout The timeout used for calls like i2c_master_cmd_begin() to the underlying driver.
         */
        I2CWrite(const std::vector<uint8_t> &bytes, std::chrono::milliseconds driver_timeout = std::chrono::milliseconds(1000));

    protected:
        /**
         * Write the address and set the read bit to 0 to issue the address and request a write.
         * Then write the bytes.
         *
         * @param handle The initialized I2C command handle.
         * @param i2c_addr The I2C address of the slave.
         */
        void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) override;

        /**
         * Set the value of the promise to unblock any callers waiting on it.
         */
        void process_result() override;

    private:
        /**
         * The bytes to write.
         */
        std::vector<uint8_t> bytes;
    };

    /**
     * Implementation for simple I2C reads, which can be executed by \c I2CMaster::transfer().
     * It stores the bytes to be read as a vector to be returned later via a future.
     */
    class I2CRead : public I2CTransfer<std::vector<uint8_t>>
    {
    public:
        /**
         * @param The number of bytes to read.
         * @param driver_timeout The timeout used for calls like i2c_master_cmd_begin() to the underlying driver.
         */
        I2CRead(size_t size, std::chrono::milliseconds driver_timeout = std::chrono::milliseconds(1000));

    protected:
        /**
         * Write the address and set the read bit to 1 to issue the address and request a read.
         * Then read into bytes.
         *
         * @param handle The initialized I2C command handle.
         * @param i2c_addr The I2C address of the slave.
         */
        void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) override;

        /**
         * Set the return value of the promise to unblock any callers waiting on it.
         */
        std::vector<uint8_t> process_result() override;

    private:
        /**
         * The bytes to read.
         */
        std::vector<uint8_t> bytes;
    };

    /**
     * This kind of transfer uses repeated start conditions to chain transfers coherently.
     * In particular, this can be used to chain multiple single write and read transfers into a single transfer with
     * repeated starts as it is commonly done for I2C devices.
     * The result is a vector of vectors representing the reads in the order of how they were added using add_read().
     */
    class I2CComposed : public I2CTransfer<std::vector<std::vector<uint8_t>>>
    {
    public:
        I2CComposed(std::chrono::milliseconds driver_timeout = std::chrono::milliseconds(1000));

        /**
         * Add a read to the chain.
         *
         * @param size The size of the read in bytes.
         */
        void add_read(size_t size);

        /**
         * Add a write to the chain.
         *
         * @param bytes The bytes to write; size will be bytes.size()
         */
        void add_write(std::vector<uint8_t> bytes);

    protected:
        /**
         * Write all chained transfers, including a repeated start issue after each but the last transfer.
         *
         * @param handle The initialized I2C command handle.
         * @param i2c_addr The I2C address of the slave.
         */
        void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) override;

        /**
         * Creates the vector with the vectors from all reads.
         */
        std::vector<std::vector<uint8_t>> process_result() override;

    private:
        class CompTransferNode
        {
        public:
            virtual ~CompTransferNode() {}
            virtual void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) = 0;
            virtual void process_result(std::vector<std::vector<uint8_t>> &read_results) {}
        };

        class CompTransferNodeRead : public CompTransferNode
        {
        public:
            CompTransferNodeRead(size_t size) : bytes(size) {}
            void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) override;

            void process_result(std::vector<std::vector<uint8_t>> &read_results) override;

        private:
            std::vector<uint8_t> bytes;
        };

        class CompTransferNodeWrite : public CompTransferNode
        {
        public:
            CompTransferNodeWrite(std::vector<uint8_t> bytes) : bytes(bytes) {}
            void queue_cmd(I2CCommandLink &handle, I2CAddress i2c_addr) override;

        private:
            std::vector<uint8_t> bytes;
        };

        /**
         * The chained transfers.
         */
        std::list<std::shared_ptr<CompTransferNode>> transfer_list;
    };

    template <typename TReturn>
    I2CTransfer<TReturn>::I2CTransfer(std::chrono::milliseconds driver_timeout_arg)
        : driver_timeout(driver_timeout_arg) {}

    template <typename TReturn>
    TReturn I2CTransfer<TReturn>::do_transfer(I2CNumber i2c_num, I2CAddress i2c_addr)
    {
        I2CCommandLink cmd_link;

        queue_cmd(cmd_link, i2c_addr);

        cmd_link.stop();

        cmd_link.execute_transfer(i2c_num, driver_timeout);

        return process_result();
    }

    template <typename TransferT>
    std::future<typename TransferT::TransferReturnT> I2CMaster::transfer(I2CAddress i2c_addr, std::shared_ptr<TransferT> xfer)
    {
        if (!xfer)
            throw I2CException(ESP_ERR_INVALID_ARG);

        return std::async(
            std::launch::async, [this](std::shared_ptr<TransferT> xfer, I2CAddress i2c_addr)
            { return xfer->do_transfer(i2c_num, i2c_addr); },
            xfer, i2c_addr);
    }

} // IDF ///

/**
 * @brief Configure IO Pad as General Purpose IO,
 *        so that it can be connected to internal Matrix,
 *        then combined with one or more peripheral signals.
 *
 * @param iopad_num
 */
void i2c_r_gpio_select_gpio(uint32_t iopad_num);

/**
 * @brief Configure GPIO direction,such as output_only,input_only,output_and_input
 *
 * @param gpio_num  Configure GPIO pins number, it should be GPIO number.
 *                  If you want to set direction of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 * @param mode      GPIO direction
 * @return esp_err_t
 */
esp_err_t set_gpio_dir(gpio_num_t gpio_num, gpio_mode_t mode);

/**
 * @brief Set the gpio level object
 *
 * @param gpio_num      -- GPIO number. If you want to set the output level of e.g. GPIO16, gpio_num should be GPIO_NUM_16 (16);
 * @param level         -- Output level. 0: low ; 1: high
 * @return esp_err_t
 */
esp_err_t set_gpio_level(gpio_num_t gpio_num, uint32_t level);

/**
 * @brief Read bytes from I2C internal buffer. When the I2C bus receives data, the ISR will copy them
 *        from the hardware RX FIFO to the internal ringbuffer.
 *        Calling this function will then copy bytes from the internal ringbuffer to the `data` user buffer.
 *        @note This function shall only be called in I2C slave mode.
 *
 * @param i2c_num       I2C port number
 * @param data          Buffer to fill with ringbuffer's bytes
 * @param max_size      Maximum bytes to read
 * @param ticks_to_wait Maximum waiting ticks (M/S)
 *
 * @return
 *     - ESP_FAIL(-1) Parameter error
 *     - Others(>=0) The number of data bytes read from I2C slave buffer.
 */
int slave_read_buffer(i2c_port_t i2c_num, uint8_t *data, size_t max_size, TickType_t ticks_to_wait);

/**
 * @brief reset I2C tx hardware fifo
 *
 * @param i2c_num        I2C port number
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 */
esp_err_t reset_rx_fifo(i2c_port_t i2c_num);

#endif