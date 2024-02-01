import pandas as pd
import argparse


def process_data(input_file, output_file):
    # read data from CSV

    df = pd.read_csv(
        input_file,
        skiprows=1,
        names=[
            "Source Host",
            "Destination Host",
            "Source Port",
            "Destination Port",
            "Count",
            "Bytes",
        ],
    )

    # aggregate data

    src_df = (
        df.groupby("Source Host")
        .agg({"Count": "sum", "Bytes": "sum"})
        .rename(columns={"Count": "Sent Packets", "Bytes": "Sent Bytes"})
    )

    dst_df = (
        df.groupby("Destination Host")
        .agg({"Count": "sum", "Bytes": "sum"})
        .rename(columns={"Count": "Received Packets", "Bytes": "Received Bytes"})
    )

    # merge and save data to CSV

    result_df = pd.concat([src_df, dst_df], axis=1).fillna(0)

    result_df.columns = [
        "Sent Packets",
        "Sent Bytes",
        "Received Packets",
        "Received Bytes",
    ]

    result_df.to_csv(output_file, index_label="IP Address")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process pcap stats data")
    parser.add_argument("-i", "--input", help="Input CSV file", required=True)
    parser.add_argument("-o", "--output", help="Output CSV file", required=True)

    args = parser.parse_args()

    process_data(args.input, args.output)
