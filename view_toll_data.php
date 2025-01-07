<?php
// Check if the POST request contains the required data
if (isset($_POST['rfid_id']) && isset($_POST['access_status'])) {
    // Get the data from the POST request
    $rfid_id = $_POST['rfid_id'];
    $access_status = $_POST['access_status'];

    // Establish a connection to the database
    $conn = new mysqli('localhost', 'your_username', 'your_password', 'your_database_name');

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Prepare an SQL statement to insert data into the toll_data table
    $stmt = $conn->prepare("INSERT INTO toll_data (rfid_id, access_status, timestamp) VALUES (?, ?, NOW())");

    // Bind the variables to the prepared statement as parameters
    $stmt->bind_param("ss", $rfid_id, $access_status);

    // Execute the prepared statement and check for success
    if ($stmt->execute()) {
        echo "New record created successfully!<br>";
    } else {
        echo "Error: " . $stmt->error . "<br>";
    }

    // Close the prepared statement to free resources
    $stmt->close();
}

// Establish a new database connection for retrieving data
$conn = new mysqli('localhost', 'your_username', 'your_password', 'your_database_name');

// Check if the connection was successful
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// SQL query to retrieve all records from the toll_data table
$sql = "SELECT * FROM toll_data";
$result = $conn->query($sql); // Execute the query
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0" >
    <title>Toll System Data</title>
    <style>
        body {
            background-color: #FCFBF4; /* Hex code for cream color */
        }

        /* Styles for the table */
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black; /* Add borders to table cells */
        }
        th, td {
            padding: 8px; /* Add padding to table cells */
            text-align: left; /* Align text to the left */
        }
    </style>
</head>
<body>
    <h1>Parking Toll Data</h1>
    <!-- Display the data in a table -->
    <table>
        <thead>
            <tr>
                <!-- Table headers -->
                <th>ID:</th>
                <th>RFID_ID:</th>
                <th>Status:</th>
                <th>Time:</th>
            </tr>
        </thead>
        <tbody>
            <?php
            // Check if there are rows in the result
            if ($result->num_rows > 0) {
                // Loop through each row and display it in the table
                while ($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo "<td>" . htmlspecialchars($row['id']) . "</td>"; // Display ID
                    echo "<td>" . htmlspecialchars($row['rfid_id']) . "</td>"; // Display RFID ID
                    echo "<td>" . htmlspecialchars($row['access_status']) . "</td>"; // Display access status
                    echo "<td>" . htmlspecialchars($row['timestamp']) . "</td>"; // Display timestamp
                    echo "</tr>";
                }
            } else {
                // Display a message if no records are found
                echo "<tr><td colspan='4'>No records found</td></tr>";
            }
            ?>
        </tbody>
    </table>
</body>
</html>
