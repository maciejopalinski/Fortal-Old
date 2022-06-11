class Node<T = int> {
    private T data;
    private Node next = null;
    private Node prev = null;

    constructor(T data) {
        setData(data);
    }

    T getData() {
        return this.data;
    }

    void setData(T data) {
        this.data = data;
    }

    Node getNext() {
        return this.next;
    }

    void setNext(Node next) {
        this.next = next;
    }

    Node getPrev() {
        return this.prev;
    }

    void setPrev(Node prev) {
        this.prev = prev;
    }
}

class LinkedList<T = int> {

    int size = 0;
    Node<T> head = null;
    Node<T> tail = null;

    int size() {
        return this.size;
    }

    private void setSize(int size) {
        if (size < 0) this.size = 0;
        else this.size = size;
    }

    void push(Node<T> node) {
        if (this.empty()) {
            node.setNext(null);
            node.setPrev(null);
            this.head = node;
            this.tail = node;
        }
        else {
            node.setNext(null);
            node.setPrev(this.tail);
            this.tail.setNext(node);
            this.tail = this.tail.getNext();
        }

        this.setSize(this.size + 1);
    }

    void push(Node<T>[] nodes) {
        for (Node<T> node : nodes) {
            this.push(node);
        }
    }

    void pushFront(Node<T> node) {
        if (this.empty()) {
            this.push(node);
        }
        else {
            node.setNext(this.head);
            node.setPrev(null);
            this.head.setPrev(node);
            this.head = node;

            this.setSize(this.size + 1);
        }

    }

    void pushFront(Node<T>[] nodes) {
        for (Node<T> node : nodes) {
            this.pushFront(node);
        }
    }

    void empty() {
        return this.size() < 1;
    }

    void clear() {
        this.head = null;
        this.tail = null;
        this.setSize(0);
    }

    bool contains(Node<T> node) {
        auto current_node = this.head;
        while (current_node) {
            if (current_node == node) {
                return true;
            }
            current_node = current_node.getNext();
        }
        return false;
    }

    Node<T> get(int index) {
        if (index < 0 || index + 1 > this.size) {
            return null;
        }

        auto current_node = this.head;
        int current_index = 0;

        if (index <= this.size / 2) {
            while (current_index != index) {
                current_node = current_node.getNext();
                current_index++;
            }
        }
        else {
            current_node = this.tail;
            current_index = this.size - 1;

            while (current_index != index) {
                current_node = current_index.getPrev();
                current_index--;
            }
        }

        return current_node;
    }

    Node<T> head() {
        return this.head;
    }

    Node<T> tail() {
        return this.tail;
    }

    Node<T> pop() {
        auto node = this.tail;
        this.tail = this.tail.getPrev();
        return node;
    }

    Node<T> popFront() {
        auto node = this.head;
        this.head = this.head.getNext();
        return node;
    }

    Node<T> remove(int index) {
        auto node = this.get(index);
        if (!node) return null;

        auto prev = node.getPrev();
        auto next = node.getNext();

        prev.setNext(next);
        next.setPrev(prev);

        return node;
    }

    void reverse() {
        Node<T> tmp = null;
        Node<T> current = this.head;

        while (current) {
            tmp = current.getPrev();
            current.setPrev(current.getNext());
            current.setNext(tmp);
            current = current.getPrev();
        }

        if (tmp) {
            this.head = tmp.getPrev();
        }
    }

    String toString() {
        String string;
        auto current = this.head;

        while (current) {
            string += current.getData();
            string += " ";
        }

        return string;
    }
}

int main() {
    LinkedList list = new LinkedList();

    list.push(1);
    list.push(2);
    list.push(3);
    list.push(4);
    list.push(5);
    list.push(6);

    printf(list.toString());

    list.reverse();

    printf(list.toString());
}